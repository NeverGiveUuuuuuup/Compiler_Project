#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <math.h>
#include <map>

#include "IR.h"
#include "IRMutator.h"
#include "IRPrinter.h"
#include "IRVisitor.h"
#include "type.h"
#include "CCPrinter.h"

using namespace Boost::Internal;

const int INF = 64;

bool compare_str(const std::string& s1, const std::string& s2){
    int i = 0, j = 0;
    while(i < s1.size() && j < s2.size()){
        if(s1[i] == ' ')++i;
        else if(s2[j] == ' ') ++j;
        else{
            if(s1[i] == s2[j]) ++i, ++j;
            else return false;
        }
    }
    if(i == s1.size() &&  j == s2.size()) return true;
    else return false;
}

void erase_blank(std::string& s1){
    std::string tmp;
    for(int i = 0; i < s1.size(); ++i){
        if(s1[i] != ' ') tmp += s1[i]; 
    }
    s1 = tmp;
}

class Case{
public:
    std::string name;
    std::vector<std::string> ins;
    std::vector<std::string> outs;
    std::string instr;
    std::string outstr;
    std::string data_type;
    std::string kernel;
    std::string grad_to;
    Case(){};
    Case(const std::vector<std::string> & m){
        int i = 0;
        for(; i < m.size(); ++i){
            if(i == 0) name = m[i];
            else if(i == 1){
                ins = handle(m[i]);
            }
            else if(i == 2){
                outs = handle(m[i]);
            }
            else if(i == 3) data_type = m[i];
            else if(i==4){ kernel = m[i];}
            else{
                grad_to = m[i];
            }
        }
    }
    std::vector<std::string> handle(const std::string& m){
        std::vector<std::string> result;
        int index1 = 0, index2 = -1;
        while((index1 = m.find("\"", index1)) != -1){
            index2 = m.find("\"", index1+1);
            result.push_back(m.substr(index1+1, index2-index1-1));
            index1 = index2+1;
        }
        return result;
    }

    void print(){
        std::cout << name << std::endl;
        std::cout << data_type << std::endl;
        std::cout << kernel << std::endl;
        for(int i = 0; i < ins.size(); ++i) std::cout << ins[i] << " ";
        std::cout << std::endl;
        for(int i = 0; i < outs.size(); ++i) std::cout << outs[i] << " ";
        std::cout << std::endl;
    }

};

Case parse_json(std::string src){
    std::ifstream ifile(src, std::ios::in);
    if(!ifile.is_open()){
        std::cout << "Parse Json Error" << std::endl;
    }
    char buffer[302];
    int colcnt = 0;
    std::vector<std::string> content;
    while(!ifile.eof()){
        ifile.getline(buffer, 300);
        std::string tmp = buffer;
        if((tmp.find("{") != -1) || (tmp.find("}") != -1)) continue;
        int pos = tmp.find(":");
        tmp = tmp.substr(pos+1);
        int pos1 = -1, pos2 = -1;
        if(colcnt == 1 || colcnt == 2){
            pos1 = tmp.find_first_of("[");
            pos2 = tmp.find_last_of("]");
            tmp = tmp.substr(pos1, pos2-pos1+1);
        }
        else{
            pos1 = tmp.find_first_of("\"");
            pos2 = tmp.find_last_of("\"");
            tmp = tmp.substr(pos1+1,pos2-pos1-1);
        }
        content.push_back(tmp);
        std::cout << tmp << std::endl;
        ++colcnt;
    }
    Case result = Case(content);
    return result;
}

Case example;

// 0P 1S 2LHS 3RHS 4TRef 5SRef 6Const 7CList 8AList 9 Op 10 IdExpr 11 Id
enum nodetype {P, S, LHS, RHS, TRef, SRef, Const, CList, AList, Op, IdExpr, Id};
std::vector<std::string> nodetype2name({"P", "S", "LHS", "RHS", "TRef", "SRef", "Const", 
    "CList", "AList", "Op", "IdExpr", "Id"});

Type index_type = Type::int_scalar(32);
Type bool_type = Type::uint_scalar(32);
Type data_type;


std::map<std::string, Expr> loop_indexs;
std::map<std::string, Expr> inputs;
std::map<std::string, Expr> outputs;

//总体的bandcheck's map 这两个结构作废了
std::map<std::string, int> bandcheck_dom;
std::map<std::string, Expr> bandcheck_expr;

// for temp
class Temp_for {
public:
  std::string str;
  int n_or_p;
  int type;
  Expr ep;
  std::map<std::string, Expr> list;             // 记录单个的index用于加减项目for循环
  std::map<std::string, Expr> bandcheck_list;   // for temp: inde for bandcheck
  std::map<std::string, int> bandcheck_value;  // for temp: dom for index for bandcheck
  Temp_for(std::string a, int b, Expr e, std::map<std::string, Expr> elist, int type, std::map<std::string, Expr> bandcheck, std::map<std::string, int> bandcheck_value){
    str = a, n_or_p = b;
    ep = e, list = elist;
    bandcheck_list = bandcheck;
    this->bandcheck_value = bandcheck_value;
    this->type = type;
  }
  Temp_for& operator=(const Temp_for& other) {
    ep = other.ep, str = other.str;
    n_or_p = other.n_or_p, type = other.type;
    list = other.list;
    bandcheck_list = other.bandcheck_list;
    bandcheck_value = other.bandcheck_value;
    return *this;
  }
};

std::vector<Temp_for> for_temp_list;

class AST;
void find_item(AST *RHS, int p_or_n);


class AST {
public:
    nodetype t;           // type of this node
    std::string str;      // str of this node
    std::vector<AST> child;
    AST* father;
    Expr ep;            //
    std::vector<Stmt> sp;
    Group gp;
    std::map<std::string, Expr> expr_index_list;    //for temp: index in ep
    std::map<std::string, Expr> bandcheck_list;
    std::map<std::string, int> bandcheck_value;    //用于记录bandcheck的范围
    float value; // only meaningful for const

    AST(){};
    AST(nodetype a, std::string b){
        t = a, str = b;
        std::cout << a << " " << str << std::endl;
    };

    AST& operator=(const AST& other){
        t = other.t;
        str = other.str;
        child = other.child;
        father = other.father;
        return *this;
    }

    void build_tree(){
        if(t == 3 && child.size() > 0) ;
        else build();
        for(int i = 0; i < child.size(); ++i) child[i].build_tree();
    }

    void travel(){
        std::cout << t << " " << nodetype2name[t] << " " << str << std::endl;
        for(int i = 0; i < child.size(); ++i) child[i].travel();
    }

    void build(){
        switch(t){
            case P: build_P();break;
            case S: build_S();break;
            case LHS: build_LHS(); break;
            case RHS: build_RHS(); break;
            case TRef: build_TRef(); break;
            case SRef: build_SRef(); break;
            case CList: build_CList(); break;
            case AList: build_AList(); break;
            case IdExpr: build_IdExpr(); break;
            default: std::cout << nodetype2name[t] << " Not Implemented !" << std::endl;
        }
    }

    void build_P(){
        int s1 = 0, s2 = -1;
        std::string tmp;
        while((s2 = str.find(";", s1)) != -1){
            tmp = str.substr(s1, s2 - s1+1);
            child.push_back(AST((nodetype) 1, tmp));
            (child.back()).father = this;
            s1 = s2+1;
        }
    }

    void build_S(){
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("=", 0);
        tmp = str.substr(0, s1);
        child.push_back(AST((nodetype) 2, tmp));
        (child.back()).father = this;
        s2 = str.find(";", s1+1);
        tmp = str.substr(s1+1, s2-1-s1-1+1);
        child.push_back(AST((nodetype) 3, tmp));
        (child.back()).father = this;
    }

    void build_LHS(){
        child.push_back(AST((nodetype) 4, str));
        (child.back()).father = this;
    }

    void build_RHS(){
        int s1 = -1, s2 = -1;
        std::string ops = "+-   */%"; // operator priority
        std::stack<std::string> v1;
        std::vector<AST> v2;
        std::string tmp;
        AST ch;
        int i = 0;
        while(i < str.size()){
            char c = str[i];
            if((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z')){
                s1 = str.find_first_of("<", i+1);
                s2 = str.find_first_of(">", s1+1);
                tmp = str.substr(s1, s2-s1+1);
                erase_blank(tmp);
                if(compare_str(tmp, "<1>")) ch = AST((nodetype) 5, str.substr(i, s2-i+1));
                else{
                    s1 = str.find_first_of("[", s2+1);
                    s2 = str.find_first_of("]", s1+1);
                    ch = AST((nodetype) 4, str.substr(i, s2-i+1));
                }
                i = s2+1;
                v2.push_back(ch);
            }
            else if(c >= '0' && c <= '9'){
                s1 = i;
                while(true){
                    ++s1;
                    if(str[s1] == '.' || (str[s1] >= '0' && str[s1] <= '9')) continue;
                    else break;
                }
                tmp = str.substr(i, s1-i);
                ch = AST((nodetype) 6, tmp);
                v2.push_back(ch);
                i = s1;
            }
            else if(c == '('){
                v1.push("(");
                ++i;
            }
            else if(c == ')'){
                while(v1.top() != "("){
                    RHS_helper(v1, v2);
                }
                v1.pop();
                ++i;
            }
            else if(c == '*' || c == '-' || c == '/' || c == '+' || c == '%'){
                std::string tmpop(1, c);
                if(c == '/' && i+1 < str.size()){
                    if(str[i+1] == '/') {
                        tmpop = "//";
                        ++i; // handle "//" as normal "/""
                    }
                }
                int c_pri = ops.find(tmpop);
                int top_pri = -1;
                while(!v1.empty()){
                    top_pri = ops.find(v1.top());
                    if(c_pri + 2 < top_pri){
                       RHS_helper(v1, v2);
                    }
                    else break;
                }
                v1.push(tmpop);
                ++i;
            }
            else ++i;
        }
        while(!v1.empty()){
            RHS_helper(v1, v2);
        }
        if(!v2.empty()){
            if(v2.size() == 1){
                if(v2[0].t != 3){
                    child.push_back(v2[0]);
                    v2[0].father = this;
                    v2.pop_back();
                }
            }
            else std::cout << "surprise at build_RHS" << std::endl;
        }
    }

    void RHS_helper(std::stack<std::string>& v1, std::vector<AST>& v2){
        AST ch2 = v2.back();
        v2.pop_back();
        AST ch1 = v2.back();
        v2.pop_back();
        std::string tmp = ch1.str + +" " + v1.top() + " " + ch2.str;
        AST op = AST((nodetype) 9,  v1.top());
        v1.pop();
        if(!v1.empty()){
            if(v1.top() == "(") tmp = "(" + tmp + ")";
        } 
        AST* ch3 = new AST((nodetype) 3, tmp);
        if(compare_str(str, tmp)) ch3 = this;
        ch3->child.push_back(ch1);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(op);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(ch2);
        (ch3->child.back()).father = ch3;
        v2.push_back(*ch3);
    }

    void build_TRef(){
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("<", 0);
        tmp = str.substr(0, s1); // tensor name fix me
        erase_blank(tmp);

        child.push_back(AST((nodetype) 11, tmp));
        (child.back()).father = this;

        s2 = str.find(">", s1+1);
        tmp = str.substr(s1+1, s2-s1-1);
        child.push_back(AST((nodetype) 7, tmp));
        (child.back()).father = this;

        s1 = str.find("[", s2+1);
        s2 = str.find("]", s1+1);
        tmp = str.substr(s1+1, s2-s1-1);
        child.push_back(AST((nodetype) 8, tmp));
        (child.back()).father = this;
    }

    void build_SRef(){
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("<", 0);
        tmp = str.substr(0, s1); // scalar name fix me
        child.push_back(AST((nodetype) 11, tmp));
        (child.back()).father = this;

        s2 = str.find(">", s1+1);
        tmp = str.substr(s1+1, s2-s1-1);
        child.push_back(AST((nodetype) 7, tmp));
        (child.back()).father = this;
    }

    void build_CList(){
        int i = 0;
        std::string tmp = "";
        AST ch;
        for(; i < str.size(); ++i){
            if(str[i] >= '0' && str[i] <= '9') tmp += str[i];
            else if(str[i] == ','){
                child.push_back(AST((nodetype) 6, tmp));
                child.back().father = this;
                tmp = "";
            }
        }
        if(tmp != "") child.push_back(AST((nodetype) 6, tmp));
        child.back().father = this;
    }

    void build_AList(){
        int i = 0;
        std::string tmp;
        for(; i < str.size(); ++i){
            if(str[i] == ' ') continue;
            else if(str[i] == ','){
                child.push_back(AST((nodetype) 10, tmp)); // IdExpr
                child.back().father = this;
                tmp = "";
            }
            else tmp += str[i];
        }
        if(tmp != ""){
            child.push_back(AST((nodetype) 10, tmp)); // IdExpr
            child.back().father = this;
        }
    }

    void build_IdExpr(){
        int s1 = -1;
        std::string ops = "+-    */%";
        std::stack<std::string> v1;
        std::vector<AST> v2;
        std::string tmp;
        AST ch = AST();
        int i = 0;
        while(i < str.size()){
            char c = str[i];
            if((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z')){
                s1 = i;
                while(true){
                    ++s1;
                    if((str[s1] >= 'A' && str[s1] <= 'Z')||(str[s1] >= 'a' && str[s1] <= 'z')) continue;
                    break;
                }
                tmp = str.substr(i, s1 - i);
                ch = AST((nodetype) 10, tmp); // Id
                v2.push_back(ch);
                i = s1;
            }
            else if(c >= '0' && c <= '9'){
                s1 = i;
                while(true){
                    ++s1;
                    if(str[s1] >= '0' && str[s1] <= '9') continue;
                    else break;
                }
                tmp = str.substr(i, s1-i);
                ch = AST((nodetype) 6, tmp); // Const
                v2.push_back(ch);
                i = s1;
            }
            else if(c == '('){
                v1.push("(");
                ++i;
            }
            else if(c == ')'){
                while(v1.top() != "("){
                    IdExpr_helper(v1, v2);
                }
                v1.pop();
                ++i;
            }
            else if(c == '*' || c == '-' || c == '/' || c == '+' || c == '%'){
                std::string tmpop(1, c);
                if(c == '/' && i+1 < str.size()){
                    if(str[i+1] == '/'){
                        tmpop = "//";
                        ++i; // handle "//" as normal "/""
                    }
                }
                int c_pri = ops.find(c);
                int top_pri = -1;
                while(!v1.empty()){
                    top_pri = ops.find(v1.top());
                    if(c_pri + 2 < top_pri){
                        IdExpr_helper(v1, v2);
                    }
                    else break;
                }
                v1.push(tmpop);
                ++i;
            }
            else ++i;
        }
        while(!v1.empty()){
            IdExpr_helper(v1, v2);
        }
    }

    void IdExpr_helper(std::stack<std::string>& v1, std::vector<AST>& v2){
        AST ch2 = v2.back();
        v2.pop_back();
        AST ch1 = v2.back();
        v2.pop_back();
        std::string tmp = ch1.str + +" " + v1.top() + " " + ch2.str;
        AST op = AST((nodetype) 9, v1.top());
        v1.pop();
        if(!v1.empty()){
            if(v1.top() == "(") tmp = "(" + tmp + ")";
        } 
        AST* ch3 = new AST((nodetype) 10, tmp);
        if(compare_str(str, tmp)) ch3 = this;
        ch3->child.push_back(ch1);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(op);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(ch2);
        (ch3->child.back()).father = ch3;
        v2.push_back(*ch3);
    }


    void build_IR(){
      for (int i = 0; i < child.size(); ++i)
        child[i].build_IR();
      std::cout << t << " " << nodetype2name[t] << " " << str << std::endl;
        IR();
    }

    void IR(){
        switch(t){
            case P: IR_P(); break;
            case S: IR_S(); break;
            case LHS: IR_LHS(); break;
            case RHS: IR_RHS(); break;
            case TRef: IR_TRef(); break;
            case SRef: IR_SRef(); break;
            case CList: IR_CList(); break;
            case AList: IR_AList(); break;
            case IdExpr: IR_IdExpr(); break;
            case Const: IR_Const(); break;
           // case Id: IR_Id(); break;
            default: std::cout << nodetype2name[t] << " Not Implemented !" << std::endl;
        }
    }

    void IR_P(){
        std::vector<Stmt> sp_list;
        for(int i = 0; i < child.size(); ++i){
            
            for (int j = 0; j < child[i].sp.size();++j) {
                std::cout << (int) child[i].sp[j].node_type() << std::endl;
                sp_list.push_back(child[i].sp[j]);
            }
        }
        std::vector<Expr> ins;
        std::map<std::string, Expr>::iterator p;
        for(p = inputs.begin(); p != inputs.end(); ++p) ins.push_back(p->second);
        std::vector<Expr> outs;
        for(p = outputs.begin(); p != outputs.end(); ++p) outs.push_back(p->second);
        std::cout << "IR P " << str << std::endl;
        std::cout << "insize " << ins.size() <<  "outsize " << outs.size() << "stmt size " << sp_list.size() << std::endl;
        gp = Kernel::make(example.name, ins, outs, sp_list, KernelType::CPU);
        inputs.clear();
        outputs.clear();
    }

    void IR_S(){        
        std::cout << "_____________***************______________" << std::endl;
        find_item(&(child[1]), 0);
        if(!for_temp_list.size()) {std::cout << "竟然没有检查出来" << std::endl<< std::endl;
        }else{
            std::cout << "竟然检查出来 加减项和对应的expr_index" << std::endl<< std::endl;
        }
        for (int i = 0; i < for_temp_list.size(); ++i) {
          std::cout << for_temp_list[i].str <<"****"<<nodetype2name[for_temp_list[i].type]<< std::endl;
          for (std::map<std::string, Expr>::iterator j = for_temp_list[i].list.begin(); j != for_temp_list[i].list.end(); ++j) {
              std::cout << j->first << std::endl;
          }
          std::cout << "+++++++bindcheck_list+++++++" << std::endl;
          for (std::map<std::string, Expr>::iterator j = for_temp_list[i].bandcheck_list.begin(); j != for_temp_list[i].bandcheck_list.end(); ++j) {
              std::cout << j->first <<"%%%%" << for_temp_list[i].bandcheck_value[j->first] << std::endl;
          }
        }
        std::cout << "_____________***************______________" << std::endl;
        // Stmt true_stmt = Move::make(child[0].ep, child[1].ep, MoveType::MemToMem);
        Stmt fake_stmt = Move::make(child[0].ep, child[0].ep, MoveType::MemToMem);
        
        // std::vector<Expr> tmp; // indexs for loop
        // for(std::map<std::string, Expr>::iterator p = loop_indexs.begin(); p != loop_indexs.end(); ++p){
        //     //std::cout << p->first << " ";
        //     tmp.push_back(p->second);
        // }

        
        //Stmt main_stmt = IfThenElse::make(cond_all, true_stmt, fake_stmt);
        //------------------- 
        // 构建最外层的for循环的body_list
        std::vector<Stmt> body_list;
        //creat temp (只能到LHS下的TRef里去找东西构建)
        Expr temp;
        std::vector<size_t> bound_list;
        for(int i = 0; i < child[0].child[0].child[1].child.size(); ++i){
            //std::cout << child[1].child[1].child[1].child[i].value << " " ;
            bound_list.push_back((size_t) child[0].child[0].child[1].child[i].value);
        }
        std::vector<Expr> index_list;
        for(int i = 0; i < child[0].child[0].child[2].child.size(); ++i){
            //std::cout << child[1].child[1].child[2].child[i].str << " "  ;
            index_list.push_back(child[0].child[0].child[2].child[i].ep);
        }


        temp = Var::make(data_type, "temp", index_list, bound_list);
        Expr cond_temp, cond1_temp, cond2_temp;
        cond1_temp = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), index_list[0]);
        cond2_temp = Compare::make(data_type, CompareOpType::LT, index_list[0], Expr(int(bound_list[0])));
        cond_temp = Binary::make(bool_type, BinaryOpType::And, cond1_temp, cond2_temp);
        for(int i = 1; i < index_list.size(); ++i){
            cond1_temp = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), index_list[i]);
            cond_temp = Binary::make(bool_type, BinaryOpType::And, cond_temp, cond1_temp);
            cond2_temp = Compare::make(data_type, CompareOpType::LT, index_list[i], Expr(int(bound_list[i])));
            cond_temp = Binary::make(bool_type, BinaryOpType::And, cond_temp, cond2_temp);

        }

        //temp = 0
        Stmt a;
        a = Move::make(temp, Expr(int(0)), MoveType::MemToMem);
        a = IfThenElse::make(cond_temp, a, fake_stmt);

        body_list.push_back(a);
        //遍历for_temp_list 构造最外层循环的body_list
        for (int i = 0; i < for_temp_list.size();++i) {
            // if it is RHS or TRef. Need loopNest
            if (for_temp_list[i].type == 3 || for_temp_list[i].type == 4) {
                std::vector<Expr> index_list;
                Stmt a;
                for (std::map<std::string, Expr>::iterator j = for_temp_list[i].list.begin(); j != for_temp_list[i].list.end();++j) {
                    if(child[0].expr_index_list.find(j->first) == child[0].expr_index_list.end()) {
                        index_list.push_back(j->second);
                    }
                }
                // temp = temp +/- item
                if(for_temp_list[i].n_or_p == 0) {
                    a = Move::make(temp, Binary::make(data_type, BinaryOpType::Add, temp, for_temp_list[i].ep), MoveType::MemToMem);
                } else {
                    a = Move::make(temp, Binary::make(data_type, BinaryOpType::Sub, temp, for_temp_list[i].ep), MoveType::MemToMem);
                }
                // 加上bandcheck
                //creat cond_all
                Expr cond_all, cond1, cond2;
                std::map<std::string, int>::iterator j = for_temp_list[i].bandcheck_value.begin();
                cond1 = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), for_temp_list[i].bandcheck_list[j->first]);
                cond2 = Compare::make(data_type, CompareOpType::LT, for_temp_list[i].bandcheck_list[j->first], Expr(int(j->second)));
                cond_all = Binary::make(bool_type, BinaryOpType::And, cond1, cond2);
                while((++j) != for_temp_list[i].bandcheck_value.end()){
                    cond1 = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), for_temp_list[i].bandcheck_list[j->first]);
                    cond_all = Binary::make(bool_type, BinaryOpType::And, cond_all, cond1);
                    cond2 = Compare::make(data_type, CompareOpType::LT, for_temp_list[i].bandcheck_list[j->first], Expr(int(j->second)));
                    cond_all = Binary::make(bool_type, BinaryOpType::And, cond_all, cond2);
                }

                a = IfThenElse::make(cond_all, a, fake_stmt);
                // 加上index循环（循环可能为空） -> for(index){temp = temp +/-  item}
                a = LoopNest::make(index_list, {a});
                body_list.push_back(a);
            } else {
                Stmt a;
                // temp = temp +/- item(这里的item是常量和标量)
                if(for_temp_list[i].n_or_p == 0) {
                    a = Move::make(temp, Binary::make(data_type, BinaryOpType::Add, temp, for_temp_list[i].ep), MoveType::MemToMem);
                } else {
                    a = Move::make(temp, Binary::make(data_type, BinaryOpType::Sub, temp, for_temp_list[i].ep), MoveType::MemToMem);
                }
                // 加上bandcheck
                //a = IfThenElse::make(cond_all, a, fake_stmt);
                a = IfThenElse::make(cond_temp, a, fake_stmt);
                body_list.push_back(a);
            }
        }
        //构建最外层的两个部分 A_for 和 B_for
            //先得到LHS的index的expr_list
        std::vector<Expr> LHS_index_list;
        for (std::map<std::string, Expr>::iterator i = child[0].expr_index_list.begin(); i != child[0].expr_index_list.end(); ++i)
        {
            LHS_index_list.push_back(i->second);
        }

        // LHS = temp      
        Stmt xx = Move::make(child[0].ep, temp, MoveType::MemToMem); 
        xx = IfThenElse::make(cond_temp, xx, fake_stmt);
        body_list.push_back(xx);
        Stmt A_for = LoopNest::make(LHS_index_list, body_list);
        sp.push_back(A_for);
        //-------------------
        //std::cout << "loop index in S " << tmp.size() << std::endl;
        //sp = LoopNest::make(tmp, {main_stmt});
        loop_indexs.clear();
        bandcheck_dom.clear();
        bandcheck_expr.clear();
        for_temp_list.clear();

    }

    void IR_LHS(){
      ep = child[0].ep;

      std::map<std::string, Expr>::iterator i;
      std::cout << "i am not ok" << std::endl;
      
      // 把LHS儿子的expr_index_list合并给自己
      for (i = child[0].expr_index_list.begin();i != child[0].expr_index_list.end();i++) {
          if (expr_index_list.find(i->first) == expr_index_list.end())
            expr_index_list[i->first] = i->second;
      }

      // 把LHS儿子的bandcheck_list合并给自己
      for (i = child[0].bandcheck_list.begin();i != child[0].bandcheck_list.end();i++) {
          if (bandcheck_list.find(i->first) == bandcheck_list.end())
              bandcheck_list[i->first] = i->second;
      }
      for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end();++i) {
          if(bandcheck_value.find(i->first) == bandcheck_value.end()) {
              bandcheck_value[i->first] = i->second;
          }
      }
    }

    void IR_RHS(){
        if(child.size() == 3){
            Expr ch1 = child[0].ep;
            Expr ch2 = child[2].ep;
            if(child[1].str == "+")ep = Binary::make(data_type, BinaryOpType::Add, ch1, ch2);
            else if(child[1].str == "-")ep = Binary::make(data_type, BinaryOpType::Sub, ch1, ch2);
            else if(child[1].str == "*")ep = Binary::make(data_type, BinaryOpType::Mul, ch1, ch2);
            else if(child[1].str == "/" || child[1].str == "//")ep = Binary::make(data_type, BinaryOpType::Div, ch1, ch2);
            else if(child[1].str == "%") ep = Binary::make(data_type, BinaryOpType::Mod, ch1, ch2);
            else std::cout << "Not Implemented !" << std::endl;
        }
        else ep = child[0].ep;

        // 把RHS儿子的bandcheck_list和bandcheck_value合并给自己
        if(child.size() == 3) {
            // bandcheck_list
            for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end();++i) {
                if(bandcheck_list.find(i->first) == bandcheck_list.end()) {
                    bandcheck_list[i->first] = i->second;
                }
            }
            for (std::map<std::string, Expr>::iterator i = child[2].bandcheck_list.begin(); i != child[2].bandcheck_list.end();++i) {
                if(bandcheck_list.find(i->first) == bandcheck_list.end()) {
                    bandcheck_list[i->first] = i->second;
                }
            }
            //bandcheck_value
            for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end();++i) {
                if(bandcheck_value.find(i->first) == bandcheck_value.end()) {
                    bandcheck_value[i->first] = i->second;
                    //std::cout << "((((((((((((((()))))))))))))" << i->second << std::endl;
                }
            }
            for (std::map<std::string, int>::iterator i = child[2].bandcheck_value.begin(); i != child[2].bandcheck_value.end();++i) {
                if(bandcheck_value.find(i->first) == bandcheck_value.end()) {
                    bandcheck_value[i->first] = i->second;
                    //std::cout << "((((((((((((((()))))))))))))" << i->second << std::endl;
                }
            }
        } else {
            for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end();++i) {
                if(bandcheck_list.find(i->first) == bandcheck_list.end()) {
                    bandcheck_list[i->first] = i->second;
                }
            }
            //bandcheck_value
            for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end();++i) {
                if(bandcheck_value.find(i->first) == bandcheck_value.end()) {
                    bandcheck_value[i->first] = i->second;
                    //std::cout << "((((((((((((((()))))))))))))" << i->second << std::endl;
                }
            }
        }
        // std::cout << nodetype2name[t] << " " << str << " " << child.size() << std::endl;
        // 把RHS儿子的expr_index_list合并给自己
        std::map<std::string, Expr>::iterator i;
        if(child.size() == 3){
            for (i = child[0].expr_index_list.begin();i != child[0].expr_index_list.end();++i) {
                if (expr_index_list.find(i->first) == expr_index_list.end())
                    expr_index_list[i->first] = i->second;
                std::cout << "##1#" << i->first << std::endl;
            }
        
            for (i = child[2].expr_index_list.begin();i != child[2].expr_index_list.end();++i) {
                if(expr_index_list.find(i->first) == expr_index_list.end())
                    expr_index_list[i->first] = i->second;
                std::cout << "##1#" << i->first << std::endl;
            }
        } 
        else {
            for (i = child[0].expr_index_list.begin();i != child[0].expr_index_list.end();++i) {
                if(expr_index_list.find(i->first) == expr_index_list.end())
                    expr_index_list[i->first] = i->second;
                std::cout << "##2#" << i->first << std::endl;
            }
        }
    }

    void IR_SRef(){
        std::string sname = child[0].str;
        std::vector<size_t> bound_list;
        if(child[1].child.size() == 1) bound_list.push_back((size_t) child[1].child[0].value);
        else std::cout << "surprise at IR_SRef!" << std::endl;
        std::vector<Expr> index_list;
        ep = Var::make(data_type, sname, index_list, bound_list);
        if(sname == example.outs[0]) {
            if(outputs.find(sname) == outputs.end()) outputs[sname] = ep;
        }
        else{
            if(inputs.find(sname) == inputs.end()) inputs[sname] = ep;
        }
    }

    void IR_TRef() {
        // creat Tref
        std::string tname = child[0].str;
        std::vector<size_t> bound_list;
        for(int i = 0; i < child[1].child.size(); ++i){
            //std::cout << child[1].child[i].value << " " ;
            bound_list.push_back((size_t) child[1].child[i].value);
        }
        std::vector<Expr> index_list;
        for(int i = 0; i < child[2].child.size(); ++i){
            //std::cout << child[2].child[i].str << " "  ;
            index_list.push_back(child[2].child[i].ep);
        }
        //std::cout << tname << std::endl;
        ep = Var::make(data_type, tname, index_list, bound_list);


        if(compare_str(tname, example.outs[0])) {
            if(outputs.find(tname) == outputs.end()) outputs[tname] = ep;
        }
        else{
            if(inputs.find(tname) == inputs.end()) inputs[tname] = ep;
        }

        std::string index_name;
        int up_bound;
        for(int i = 0; i < child[1].child.size(); ++i){
          // std::cout << child[1].child[i].value << " " ;
            index_name = child[2].child[i].str;
            up_bound = child[1].child[i].value;
            if(bandcheck_dom.find(index_name) != bandcheck_dom.end()){
                if(bandcheck_dom[index_name] > up_bound){
                    bandcheck_dom[index_name] = child[1].child[i].value;
                    bandcheck_expr[index_name] = child[2].child[i].ep;
                }
            }
            else{
                bandcheck_dom[index_name] = child[1].child[i].value;
                bandcheck_expr[index_name] = child[2].child[i].ep;
            }
        }
        
         
        //std::cout << nodetype2name[t] << "______ " << str << " " << child.size() << std::endl;
        std::map<std::string, Expr>::iterator j;
        for(int i = 0; i < child[2].child.size(); ++i){
            //std::cout << child[2].child[i].str << " "  ;
            //把Alist中的所有儿子的str和ep存入bandcheck_list，用于加减项的bandcheck
            if(bandcheck_list.find(child[2].child[i].str) == bandcheck_list.end()) {
                bandcheck_list[child[2].child[i].str] = child[2].child[i].ep;
                bandcheck_value[child[2].child[i].str] = int(child[1].child[i].value);
                //std::cout << "((((((((((((((()))))))))))))" << child[1].child[i].value << std::endl;
            } else if (bandcheck_value[child[2].child[i].str] > int(child[1].child[i].value)) {
                bandcheck_value[child[2].child[i].str] = int(child[1].child[i].value);
            }
            // 把Alist中的所有儿子的expr_index_list合并存入expr_index_list
            for (j = child[2].child[i].expr_index_list.begin();j != child[2].child[i].expr_index_list.end();++j) {
                if(expr_index_list.find(j->first) == expr_index_list.end()) expr_index_list[j->first] = j->second;
                std::cout << "##1#" << j->first << std::endl;
            }
            std::cout << "###Tref"<< nodetype2name[child[2].child[i].t] << std::endl;
        }

    }

    void IR_CList(){

    }

    void IR_AList(){

    }

    void IR_IdExpr(){
      Expr dom_inf = Dom::make(index_type, -INF, 2 * INF); // fix me->Ok
      std::cout << nodetype2name[t] << "____ " << str << " " << child.size() << std::endl;
        if(child.size()){
            Expr ch1 = Expr(child[0].ep);
            Expr ch2 = Expr(child[2].ep);
            if(child[1].str == "+")ep = Binary::make(data_type, BinaryOpType::Add, ch1, ch2);
            else if(child[1].str == "-")ep = Binary::make(data_type, BinaryOpType::Sub, ch1, ch2);
            else if(child[1].str == "*")ep = Binary::make(data_type, BinaryOpType::Mul, ch1, ch2);
            else if(child[1].str == "/" || child[1].str == "//")ep = Binary::make(data_type, BinaryOpType::Div, ch1, ch2);
            else if(child[1].str == "%") ep = Binary::make(data_type, BinaryOpType::Mod, ch1, ch2);
            else std::cout << "Not Implemented Op in IdExpr!" << std::endl;
            // 往expr_index_list中放入组合加减项需要的单个index
            std::map<std::string, Expr>::iterator i ;
            for (i = child[0].expr_index_list.begin(); i != child[0].expr_index_list.end();++i)
            {
                if (expr_index_list.find(i->first) == expr_index_list.end())
                {
                    expr_index_list[i->first] = i->second;
                }
                std::cout << "##1#" << i->first << std::endl;
            }
            for (i = child[2].expr_index_list.begin(); i != child[2].expr_index_list.end();++i)
            {
                if(expr_index_list.find(i->first) == expr_index_list.end()) expr_index_list[i->first] = i->second;
                std::cout << "##1#" << i->first << std::endl;
            }
        }
        else{
            if(father && father->father){
                if(father->t == 8){
                    int pos = -1, upbound = -1;
                    for(int i = 0; i < father->child.size(); ++i){
                        if(father->child[i].str == str){
                            pos = i;
                            break;
                        }
                    }
                    if(pos != -1){
                        upbound = father->father->child[1].child[pos].value;  
                        std::cout << "fuck " << str <<  " " << upbound << std::endl;
                        dom_inf = Dom::make(index_type, 0, upbound);
                    }                
                }
            }
            ep = Index::make(index_type, str, dom_inf, IndexType::Spatial);
            if (expr_index_list.find(str) == expr_index_list.end()) {
              expr_index_list[str] = ep;
            }
            std::cout << "##2#" << str << std::endl;
            if(loop_indexs.find(str) == loop_indexs.end()) loop_indexs[str] = ep;
        }
    }
    
    void IR_Const(){
        int xsd = str.find(".");
        int zhengshu = -1, xiaoshu = -1;
        if(xsd == -1){
            zhengshu = atoi(str.c_str());
            //std::cout << zhengshu << std::endl;
            value = zhengshu;
            ep = Expr(zhengshu);
        }
        else{
            zhengshu = atoi(str.substr(0, xsd).c_str());
            int xiaoshulen = str.size()-xsd-1;
            xiaoshu = atoi(str.substr(xsd+1, xiaoshulen).c_str());
            float result = zhengshu + (xiaoshu/pow(10, xiaoshulen));
            //std::cout << result << std::endl;
            value = result;
            ep = Expr(result);
        }
    }
};
// p_or_n = 0代表正数1代表负数
void find_item(AST *RHS, int p_or_n) {
    if (RHS->child.size() == 3 && RHS->t == 3) {
        //std::cout <<nodetype2name[RHS->t]<< "^^^"<<RHS->str <<"^^^"<<nodetype2name[RHS->child[0].t] << RHS->child[0].str << "@@@"<< nodetype2name[RHS->child[2].t]<<RHS->child[1].str << "@@@"<< RHS->child[2].str << std::endl;
        std::string a = RHS->str;
        erase_blank(a);
        if(a[0] != '(' || a[a.size() - 1] != ')') {
            if (RHS->child[1].str[0] == '+') {
                find_item(&(RHS->child[0]), p_or_n);
                find_item(&(RHS->child[2]), 0);
            }
            else if (RHS->child[1].str[0] == '-') {
                find_item(&(RHS->child[0]), p_or_n);
                find_item(&(RHS->child[2]), 1);
            }
            else {
                Temp_for temp(RHS->str, p_or_n, RHS->ep, RHS->expr_index_list, RHS->t, RHS->bandcheck_list, RHS->bandcheck_value);
                for_temp_list.push_back(temp);
            }
        } else {
            Temp_for temp(RHS->str, p_or_n, RHS->ep, RHS->expr_index_list, RHS->t, RHS->bandcheck_list, RHS->bandcheck_value);
            for_temp_list.push_back(temp);
        }
    }
    else {
        //std::cout << "^^^" << RHS->str <<"%%%%"<<RHS->child[0].str << std::endl;
        if (RHS->t == 4 || RHS->t == 5 || RHS->t == 6 )
        {
            Temp_for temp(RHS->str, p_or_n, RHS->ep, RHS->expr_index_list, RHS->t, RHS->bandcheck_list, RHS->bandcheck_value);
            for_temp_list.push_back(temp);
        } else {
        find_item(&(RHS->child[0]), p_or_n);
        }
        //std::cout << "pushback:" << temp.ep << std::endl;
    }
}


int main(int argc, char* argv[]){
    std::string src, dst;
    for(int i = 0; i <= 10; ++i){
        if(i == 0) src = "./cases/example.json";
        else src = "./cases/case" + std::to_string(i) + ".json";
        if(access(src.c_str(), 0) == -1) continue;
        example = parse_json(src);
        example.print();

        dst = "./kernels/" + example.name + ".cc";
        if(example.data_type == "float") data_type = Type::float_scalar(32);
        else if(example.data_type == "int") data_type = Type::int_scalar(32);
        else std::cout << "data type in json not int or float!" << std::endl;

        AST root = AST((nodetype) 0, example.kernel);
        root.build_tree();
        std::cout << "-----" << std::endl;
        root.travel();
        std::cout << "*****" << std::endl;
        root.build_IR();

        std::cout << root.str <<std::endl;

        Group kernel = root.gp;

        // visitor
        IRVisitor visitor;
        root.gp.visit_group(&visitor);

        // mutator
        //IRMutator mutator;
        //kernel = mutator.mutate(kernel);

        // printer
        CCPrinter printer;
        std::string code = printer.print(root.gp);
        std::ofstream ofile(dst, std::ios::out);
        ofile << code;
        ofile.close();
        std::cout << "************" << std::endl;
    }
    return 0;
}