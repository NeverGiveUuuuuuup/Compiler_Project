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

//HJH:
std::vector<std::string> grad_to;
std::map<std::string, Expr> grad_to_expr;

//  比较两个字符串是否相同。（忽略空格）
bool compare_str(const std::string &s1, const std::string &s2)
{
    int i = 0, j = 0;
    while (i < s1.size() && j < s2.size())
    {
        if (s1[i] == ' ')
            ++i;
        else if (s2[j] == ' ')
            ++j;
        else
        {
            if (s1[i] == s2[j])
                ++i, ++j;
            else
                return false;
        }
    }
    if (i == s1.size() && j == s2.size())
        return true;
    else
        return false;
}

// 去掉字符串里的空格
void erase_blank(std::string &s1)
{
    std::string tmp;
    for (int i = 0; i < s1.size(); ++i)
    {
        if (s1[i] != ' ')
            tmp += s1[i];
    }
    s1 = tmp;
}

// 定义题目case的类，写入题目的各种信息
class Case
{
public:
    std::string name;
    std::vector<std::string> ins;
    std::vector<std::string> outs;
    std::string instr;
    std::string outstr;
    std::string data_type;
    std::string kernel;
    
    Case(){};
    Case(const std::vector<std::string> &m)
    {
        int i = 0;
        for (; i < m.size(); ++i)
        {
            if (i == 0)
                name = m[i];
            else if (i == 1)
            {
                ins = handle(m[i]);
            }
            else if (i == 2)
            {
                outs = handle(m[i]);
            }
            else if (i == 3)
                data_type = m[i];
            else if(i==4)
            {
                kernel = m[i];
            }
            else{
                grad_to = handle(m[i]);
            }
        }
    }
    std::vector<std::string> handle(const std::string &m)
    {
        std::vector<std::string> result;
        int index1 = 0, index2 = -1;
        while ((index1 = m.find("\"", index1)) != -1)
        {
            index2 = m.find("\"", index1 + 1);
            result.push_back(m.substr(index1 + 1, index2 - index1 - 1));
            index1 = index2 + 1;
        }
        return result;
    }
    // 打印题目信息
    void print()
    {
        std::cout << "---------------题目信息---------------" << std::endl;
        std::cout << name << std::endl;
        std::cout << data_type << std::endl;
        std::cout << kernel << std::endl;
        
        for (int i = 0; i < ins.size(); ++i)
            std::cout << ins[i] << " ";
        std::cout << std::endl;
        for (int i = 0; i < outs.size(); ++i)
            std::cout << outs[i] << " ";
        std::cout << std::endl;
        std::cout << "---------------题目信息---------------" << std::endl;
    }
};

// 分析json，并把信息填入case
Case parse_json(std::string src)
{
    std::ifstream ifile(src, std::ios::in);
    if (!ifile.is_open())
    {
        std::cout << "Parse Json Error" << std::endl;
    }
    char buffer[302];
    int colcnt = 0;
    std::vector<std::string> content;
    while (!ifile.eof())
    {
        ifile.getline(buffer, 300);
        std::string tmp = buffer;
        if ((tmp.find("{") != -1) || (tmp.find("}") != -1))
            continue;
        int pos = tmp.find(":");
        tmp = tmp.substr(pos + 1);
        int pos1 = -1, pos2 = -1;
        if (colcnt == 1 || colcnt == 2 || colcnt == 5)
        {
            pos1 = tmp.find_first_of("[");
            pos2 = tmp.find_last_of("]");
            tmp = tmp.substr(pos1, pos2 - pos1 + 1);
        }
        else
        {
            pos1 = tmp.find_first_of("\"");
            pos2 = tmp.find_last_of("\"");
            tmp = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
        }
        content.push_back(tmp);
        std::cout << tmp << std::endl;
        ++colcnt;
    }
    Case result = Case(content);
    return result;
}

Case example;



/* 0P 1S 2LHS 3RHS 4TRef 5SRef 6Const 7CList 8AList 9 Op 10 IdExpr 11 Id
 * 定义节点的类型
*/
enum nodetype
{
    P,
    S,
    LHS,
    RHS,
    TRef,
    SRef,
    Const,
    CList,
    AList,
    Op,
    IdExpr,
    Id
};

// 根据类型号得到类型名字
std::vector<std::string> nodetype2name({"P", "S", "LHS", "RHS", "TRef", "SRef", "Const",
                                        "CList", "AList", "Op", "IdExpr", "Id"});

Type index_type = Type::int_scalar(32);
Type bool_type = Type::uint_scalar(32);
Type data_type;

// 用于构造S的loop
std::map<std::string, Expr> loop_indexs;

std::map<std::string, Expr> inputs;
std::map<std::string, Expr> outputs;


// 等式整体的bandcheck。
std::map<std::string, int> bandcheck_dom; //Q: begin=0?
std::map<std::string, Expr> bandcheck_expr;

class AST;

// AST树节点的类
class AST
{
public:
    nodetype t;      // 节点类型
    std::string str; // 节点对应的字符串
    std::vector<AST> child;     //子节点
    AST *father;
    Expr ep;
    std::vector<Stmt> sp;
    Group gp;
    // std::map<std::string, Expr> expr_index_list; //for temp: index in ep
    
    // 记录当前节点的bandcheck
    std::map<std::string, Expr> bandcheck_list;  //记录bandcheck中要判断的expr
    std::map<std::string, int> bandcheck_value; //用于记录bandcheck的范围
    
    float value;                                // only meaningful for const

    AST(){};
    // 节点生成函数
    AST(nodetype a, std::string b)
    {
        t = a, str = b;
        std::cout << a << " " << str << std::endl;
    };

    AST &operator=(const AST &other)
    {
        t = other.t;
        str = other.str;
        child = other.child;
        father = other.father;
        return *this;
    }
    // 生成AST树的成员函数，进入build()各自生成。
    void build_tree()
    {
        // 如果是右值节点且有儿子
        //t==3: RHS
        if (t == 3 && child.size() > 0)
            ;
        else
            build();
        for (int i = 0; i < child.size(); ++i)
            child[i].build_tree();
    }
    // 深度优先前序遍历，输出AST树。
    void travel()
    {
        std::cout << t << " " << nodetype2name[t] << " " << str << std::endl;
        for (int i = 0; i < child.size(); ++i)
            child[i].travel();
    }

    // 不同的节点类型执行不同的生成函数
    void build()
    {
        switch (t)
        {
        case P:
            build_P();
            break;
        case S:
            build_S();
            break;
        case LHS:
            build_LHS();
            break;
        case RHS:
            build_RHS();
            break;
        case TRef:
            build_TRef();
            break;
        case SRef:
            build_SRef();
            break;
        case CList:
            build_CList();
            break;
        case AList:
            build_AList();
            break;
        case IdExpr:
            build_IdExpr();
            break;
        default:
            std::cout << nodetype2name[t] << " Not Implemented !" << std::endl;
        }
    }
    // 每个S对应一个儿子
    void build_P()
    {
        int s1 = 0, s2 = -1;
        std::string tmp;
        while ((s2 = str.find(";", s1)) != -1)
        {
            tmp = str.substr(s1, s2 - s1 + 1);
            child.push_back(AST((nodetype)1, tmp));
            (child.back()).father = this;
            s1 = s2 + 1;
        }
    }
    // 通过等号分隔，生成左值和右值两个儿子
    void build_S()
    {
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("=", 0);
        tmp = str.substr(0, s1);
        child.push_back(AST((nodetype)2, tmp));
        (child.back()).father = this;
        s2 = str.find(";", s1 + 1);
        tmp = str.substr(s1 + 1, s2 - 1 - s1 - 1 + 1);
        child.push_back(AST((nodetype)3, tmp));
        (child.back()).father = this;
    }

    // 左值的儿子就是TRef
    void build_LHS()
    {
        child.push_back(AST((nodetype)4, str));
        (child.back()).father = this;
    }

    // 每个RHS是一个算术式子。
    void build_RHS()
    {
        int s1 = -1, s2 = -1;
        std::string ops = "+-   */%"; // operator priority
        std::stack<std::string> v1;
        std::vector<AST> v2;
        std::string tmp;
        AST ch;
        int i = 0;
        while (i < str.size())
        {
            char c = str[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                s1 = str.find_first_of("<", i + 1);
                s2 = str.find_first_of(">", s1 + 1);
                tmp = str.substr(s1, s2 - s1 + 1);
                erase_blank(tmp);
                if (compare_str(tmp, "<1>"))
                    ch = AST((nodetype)5, str.substr(i, s2 - i + 1));
                else
                {
                    s1 = str.find_first_of("[", s2 + 1);
                    s2 = str.find_first_of("]", s1 + 1);
                    ch = AST((nodetype)4, str.substr(i, s2 - i + 1));
                }
                i = s2 + 1;
                v2.push_back(ch);
            }
            else if (c >= '0' && c <= '9')
            {
                s1 = i;
                while (true)
                {
                    ++s1;
                    if (str[s1] == '.' || (str[s1] >= '0' && str[s1] <= '9'))
                        continue;
                    else
                        break;
                }
                tmp = str.substr(i, s1 - i);
                ch = AST((nodetype)6, tmp);
                v2.push_back(ch);
                i = s1;
            }
            else if (c == '(')
            {
                v1.push("(");
                ++i;
            }
            else if (c == ')')
            {
                while (v1.top() != "(")
                {
                    RHS_helper(v1, v2);
                }
                v1.pop();
                ++i;
            }
            else if (c == '*' || c == '-' || c == '/' || c == '+' || c == '%')
            {
                std::string tmpop(1, c);
                if (c == '/' && i + 1 < str.size())
                {
                    if (str[i + 1] == '/')
                    {
                        tmpop = "//";
                        ++i; // handle "//" as normal "/""
                    }
                }
                int c_pri = ops.find(tmpop);
                int top_pri = -1;
                while (!v1.empty())
                {
                    top_pri = ops.find(v1.top());
                    if (c_pri + 2 < top_pri)
                    {
                        RHS_helper(v1, v2);
                    }
                    else
                        break;
                }
                v1.push(tmpop);
                ++i;
            }
            else
                ++i;
        }
        while (!v1.empty())
        {
            RHS_helper(v1, v2);
        }
        if (!v2.empty())
        {
            if (v2.size() == 1)
            {
                if (v2[0].t != 3)
                {
                    child.push_back(v2[0]);
                    v2[0].father = this;
                    v2.pop_back();
                }
            }
            else
                std::cout << "surprise at build_RHS" << std::endl;
        }
    }

    // 帮助生成RHS节点
    void RHS_helper(std::stack<std::string> &v1, std::vector<AST> &v2)
    {
        AST ch2 = v2.back();
        v2.pop_back();
        AST ch1 = v2.back();
        v2.pop_back();
        std::string tmp = ch1.str + +" " + v1.top() + " " + ch2.str;
        AST op = AST((nodetype)9, v1.top());
        v1.pop();
        if (!v1.empty())
        {
            if (v1.top() == "(")
                tmp = "(" + tmp + ")";
        }
        AST *ch3 = new AST((nodetype)3, tmp);
        if (compare_str(str, tmp))
            ch3 = this;
        ch3->child.push_back(ch1);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(op);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(ch2);
        (ch3->child.back()).father = ch3;
        v2.push_back(*ch3);
    }

    // 生成向量节点，儿子没从左到右是
    void build_TRef()
    {
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("<", 0);
        tmp = str.substr(0, s1); // tensor name fix me
        erase_blank(tmp);

        child.push_back(AST((nodetype)11, tmp));
        (child.back()).father = this;

        s2 = str.find(">", s1 + 1);
        tmp = str.substr(s1 + 1, s2 - s1 - 1);
        child.push_back(AST((nodetype)7, tmp));
        (child.back()).father = this;

        s1 = str.find("[", s2 + 1);
        s2 = str.find("]", s1 + 1);
        tmp = str.substr(s1 + 1, s2 - s1 - 1);
        child.push_back(AST((nodetype)8, tmp));
        (child.back()).father = this;
    }

    void build_SRef()
    {
        int s1 = -1, s2 = -1;
        std::string tmp;
        s1 = str.find("<", 0);
        tmp = str.substr(0, s1); // scalar name fix me
        child.push_back(AST((nodetype)11, tmp));
        (child.back()).father = this;

        s2 = str.find(">", s1 + 1);
        tmp = str.substr(s1 + 1, s2 - s1 - 1);
        child.push_back(AST((nodetype)7, tmp));
        (child.back()).father = this;
    }

    // 生成CList, 子节点是向量数字参数。
    void build_CList()
    {
        int i = 0;
        std::string tmp = "";
        AST ch;
        for (; i < str.size(); ++i)
        {
            if (str[i] >= '0' && str[i] <= '9')
                tmp += str[i];
            else if (str[i] == ',')
            {
                child.push_back(AST((nodetype)6, tmp));
                child.back().father = this;
                tmp = "";
            }
        }
        if (tmp != "")
            child.push_back(AST((nodetype)6, tmp));
        child.back().father = this;
    }

    // 生成AList, 子节点是向量字母参数。
    void build_AList()
    {
        int i = 0;
        std::string tmp;
        for (; i < str.size(); ++i)
        {
            if (str[i] == ' ')
                continue;
            else if (str[i] == ',')
            {
                child.push_back(AST((nodetype)10, tmp)); // IdExpr
                child.back().father = this;
                tmp = "";
            }
            else
                tmp += str[i];
        }
        if (tmp != "")
        {
            child.push_back(AST((nodetype)10, tmp)); // IdExpr
            child.back().father = this;
        }
    }

    // Alist里面的字母参数，可以是表达式。
    void build_IdExpr()
    {
        int s1 = -1;
        std::string ops = "+-    */%";
        std::stack<std::string> v1;
        std::vector<AST> v2;
        std::string tmp;
        AST ch = AST();
        int i = 0;
        while (i < str.size())
        {
            char c = str[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                s1 = i;
                while (true)
                {
                    ++s1;
                    if ((str[s1] >= 'A' && str[s1] <= 'Z') || (str[s1] >= 'a' && str[s1] <= 'z'))
                        continue;
                    break;
                }
                tmp = str.substr(i, s1 - i);
                ch = AST((nodetype)10, tmp); // Id
                //Q 10:IdExpr, 11:Id?
                v2.push_back(ch);
                i = s1;
            }
            else if (c >= '0' && c <= '9')
            {
                s1 = i;
                while (true)
                {
                    ++s1;
                    if (str[s1] >= '0' && str[s1] <= '9')
                        continue;
                    else
                        break;
                }
                tmp = str.substr(i, s1 - i);
                ch = AST((nodetype)6, tmp); // Const
                v2.push_back(ch);
                i = s1;
            }
            else if (c == '(')
            {
                v1.push("(");
                ++i;
            }
            else if (c == ')')
            {
                while (v1.top() != "(")
                {
                    IdExpr_helper(v1, v2);
                }
                v1.pop();
                ++i;
            }
            else if (c == '*' || c == '-' || c == '/' || c == '+' || c == '%')
            {
                std::string tmpop(1, c);
                if (c == '/' && i + 1 < str.size())
                {
                    if (str[i + 1] == '/')
                    {
                        tmpop = "//";
                        ++i; // handle "//" as normal "/""
                    }
                }
                int c_pri = ops.find(c);
                int top_pri = -1;
                while (!v1.empty())
                {
                    top_pri = ops.find(v1.top());
                    if (c_pri + 2 < top_pri)
                    {
                        IdExpr_helper(v1, v2);
                    }
                    else
                        break;
                }
                v1.push(tmpop);
                ++i;
            }
            else
                ++i;
        }
        while (!v1.empty())
        {
            IdExpr_helper(v1, v2);
        }
    }

    //帮助生成向量下标的表达式子树
    void IdExpr_helper(std::stack<std::string> &v1, std::vector<AST> &v2)
    {
        AST ch2 = v2.back();
        v2.pop_back();
        AST ch1 = v2.back();
        v2.pop_back();
        std::string tmp = ch1.str + +" " + v1.top() + " " + ch2.str;
        AST op = AST((nodetype)9, v1.top());
        v1.pop();
        if (!v1.empty())
        {
            if (v1.top() == "(")
                tmp = "(" + tmp + ")";
        }
        AST *ch3 = new AST((nodetype)10, tmp);
        if (compare_str(str, tmp))
            ch3 = this;
        ch3->child.push_back(ch1);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(op);
        (ch3->child.back()).father = ch3;
        ch3->child.push_back(ch2);
        (ch3->child.back()).father = ch3;
        v2.push_back(*ch3);
    }

    // 自底向上遍历AST树的节点，对每个节点执行IR()。
    void build_IR()
    {
        for (int i = 0; i < child.size(); ++i)
            child[i].build_IR();
        std::cout << t << " " << nodetype2name[t] << " " << str << std::endl;
        IR();
    }

    void IR()
    {
        switch (t)
        {
        case P:
            IR_P();
            break;
        case S:
            IR_S();
            break;
        case LHS:
            IR_LHS();
            break;
        case RHS:
            IR_RHS();
            break;
        case TRef:
            IR_TRef();
            break;
        case SRef:
            IR_SRef();
            break;
        case CList:
            IR_CList();
            break;
        case AList:
            IR_AList();
            break;
        case IdExpr:
            IR_IdExpr();
            break;
        case Const:
            IR_Const();
            break;
            // case Id: IR_Id(); break;
        default:
            std::cout << nodetype2name[t] << " Not Implemented !" << std::endl;
        }
    }

    void IR_P()
    {
        std::vector<Stmt> sp_list;
        for (int i = 0; i < child.size(); ++i)
        {

            for (int j = 0; j < child[i].sp.size(); ++j)
            {
                std::cout << (int)child[i].sp[j].node_type() << std::endl;
                sp_list.push_back(child[i].sp[j]);
            }
        }
        std::vector<Expr> ins;
        std::map<std::string, Expr>::iterator p;
        for (p = inputs.begin(); p != inputs.end(); ++p)
            ins.push_back(p->second);
        std::vector<Expr> outs;
        for (p = outputs.begin(); p != outputs.end(); ++p)
            outs.push_back(p->second);
        std::cout << "IR P " << str << std::endl;
        std::cout << "insize " << ins.size() << "outsize " << outs.size() << "stmt size " << sp_list.size() << std::endl;
        gp = Kernel::make(example.name, ins, outs, sp_list, KernelType::CPU);
        inputs.clear();
        outputs.clear();
    }

    void IR_S()
    {
        
        // bondcheck使用if else实现的，这个是if失败时候执行的代码
        Stmt fake_stmt = Move::make(child[0].ep, child[0].ep, MoveType::MemToMem);

        
        std::vector<Stmt> body_list;
        
        // 这里的循环体要改成我们的 dx = dA * B, 现在只是 LHS = RHS
        Stmt important_body = Move::make(child[0].ep, child[1].ep, MoveType::MemToMem);



        // 开始构造bandcheck
        // 合并两个儿子的bandcheck_list合并为本节点的bandcheck_list
        for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end(); ++i)
        {
            if (bandcheck_list.find(i->first) == bandcheck_list.end())
            {
                bandcheck_list[i->first] = i->second;
            }
        }
        for (std::map<std::string, Expr>::iterator i = child[1].bandcheck_list.begin(); i != child[1].bandcheck_list.end(); ++i)
        {
            if (bandcheck_list.find(i->first) == bandcheck_list.end())
            {
                bandcheck_list[i->first] = i->second;
            }
        }
        // 合并两个儿子的bandcheck_value合并为本节点的bandcheck_value
        for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end(); ++i)
        {
            // 如果有更小的范围就换成更小的
            if (bandcheck_value.find(i->first) == bandcheck_value.end())
            {
                bandcheck_value[i->first] = i->second;
            } else {
                if (bandcheck_value[i->first] > i->second) {
                    bandcheck_value[i->first] = i->second;
                }
            }
        }
        for (std::map<std::string, int>::iterator i = child[1].bandcheck_value.begin(); i != child[1].bandcheck_value.end(); ++i)
        {
            if (bandcheck_value.find(i->first) == bandcheck_value.end())
            {
                bandcheck_value[i->first] = i->second;
            } else {
                if (bandcheck_value[i->first] > i->second) {
                    bandcheck_value[i->first] = i->second;
                }
            }
        }
        
        std::vector<Expr> ep_list;
        std::vector<int> value_list;
        for (std::map<std::string, int>::iterator i = bandcheck_value.begin(); i != bandcheck_value.end(); ++i)
        {
            ep_list.push_back(bandcheck_list[i->first]);
            value_list.push_back(i->second);
        }


        Expr cond_temp, cond1_temp, cond2_temp;
        cond1_temp = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), ep_list[0]);
        cond2_temp = Compare::make(data_type, CompareOpType::LT, ep_list[0], Expr(int(value_list[0])));
        cond_temp = Binary::make(bool_type, BinaryOpType::And, cond1_temp, cond2_temp);
        for (int i = 1; i < ep_list.size(); ++i)
        {
            cond1_temp = Compare::make(data_type, CompareOpType::LE, Expr(int(0)), ep_list[i]);
            cond_temp = Binary::make(bool_type, BinaryOpType::And, cond_temp, cond1_temp);
            cond2_temp = Compare::make(data_type, CompareOpType::LT, ep_list[i], Expr(int(value_list[i])));
            cond_temp = Binary::make(bool_type, BinaryOpType::And, cond_temp, cond2_temp);
        }

        
        // 加上bandchek
        Stmt a;
        a = IfThenElse::make(cond_temp, important_body, fake_stmt);

        body_list.push_back(a);
        
        //HJH    
        std::vector<Expr> loop_indexs_vec;
        for (std::map<std::string, Expr>::iterator i = loop_indexs.begin(); i != loop_indexs.end(); ++i)
        {
            loop_indexs_vec.push_back(i->second);
        }
        Stmt A = LoopNest::make(loop_indexs_vec, body_list);
        sp.push_back(A);
        //-------------------
        //std::cout << "loop index in S " << tmp.size() << std::endl;
        //sp = LoopNest::make(tmp, {main_stmt});

        loop_indexs.clear();
        bandcheck_dom.clear();
        bandcheck_expr.clear();
    }

    void IR_LHS()
    {
        ep = child[0].ep;

        // 把LHS儿子的bandcheck_list给自己
        for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end(); i++)
        {
            if (bandcheck_list.find(i->first) == bandcheck_list.end())
                bandcheck_list[i->first] = i->second;
        }
        
        // 把LHS儿子的bandcheck_value给自己
        for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end(); ++i)
        {
            if (bandcheck_value.find(i->first) == bandcheck_value.end())
            {
                bandcheck_value[i->first] = i->second;
            }
        }
    }

    void IR_RHS()
    {
        // 生成此节点的ep
        if (child.size() == 3)
        {
            Expr ch1 = child[0].ep;
            Expr ch2 = child[2].ep;
            if (child[1].str == "+")
                ep = Binary::make(data_type, BinaryOpType::Add, ch1, ch2);
            else if (child[1].str == "-")
                ep = Binary::make(data_type, BinaryOpType::Sub, ch1, ch2);
            else if (child[1].str == "*")
                ep = Binary::make(data_type, BinaryOpType::Mul, ch1, ch2);
            else if (child[1].str == "/" || child[1].str == "//")
                ep = Binary::make(data_type, BinaryOpType::Div, ch1, ch2);
            else if (child[1].str == "%")
                ep = Binary::make(data_type, BinaryOpType::Mod, ch1, ch2);
            else
                std::cout << "Not Implemented !" << std::endl;
        }
        else {
            ep = child[0].ep;
        }

        // 把RHS儿子的bandcheck_list和bandcheck_value合并给自己
        if (child.size() == 3)
        {
            // 合并两个儿子的bandcheck_list合并为本节点的bandcheck_list给自己
            for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end(); ++i)
            {
                if (bandcheck_list.find(i->first) == bandcheck_list.end())
                {
                    bandcheck_list[i->first] = i->second;
                }
            }
            for (std::map<std::string, Expr>::iterator i = child[2].bandcheck_list.begin(); i != child[2].bandcheck_list.end(); ++i)
            {
                if (bandcheck_list.find(i->first) == bandcheck_list.end())
                {
                    bandcheck_list[i->first] = i->second;
                }
            }
            // 合并两个儿子的bandcheck_value合并为本节点的bandcheck_value给自己
            for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end(); ++i)
            {
                // 如果有更小的范围就换成更小的
                if (bandcheck_value.find(i->first) == bandcheck_value.end())
                {
                    bandcheck_value[i->first] = i->second;
                } else {
                    if (bandcheck_value[i->first] > i->second) {
                        bandcheck_value[i->first] = i->second;
                    }
                }
            }
            for (std::map<std::string, int>::iterator i = child[2].bandcheck_value.begin(); i != child[2].bandcheck_value.end(); ++i)
            {
                if (bandcheck_value.find(i->first) == bandcheck_value.end())
                {
                    bandcheck_value[i->first] = i->second;
                } else {
                    if (bandcheck_value[i->first] > i->second) {
                        bandcheck_value[i->first] = i->second;
                    }
                }
            }
        }
        else
        {
            // 如果下面只是一个Tref就直接传过来。
            //bandcheck_list
            for (std::map<std::string, Expr>::iterator i = child[0].bandcheck_list.begin(); i != child[0].bandcheck_list.end(); ++i)
            {
                if (bandcheck_list.find(i->first) == bandcheck_list.end())
                {
                    bandcheck_list[i->first] = i->second;
                }
            }
            //bandcheck_value
            for (std::map<std::string, int>::iterator i = child[0].bandcheck_value.begin(); i != child[0].bandcheck_value.end(); ++i)
            {
                if (bandcheck_value.find(i->first) == bandcheck_value.end())
                {
                    bandcheck_value[i->first] = i->second;
                }
            }
        }
    }

    // 处理标量，不过project2好像没有标量
    void IR_SRef()
    {
        std::string sname = child[0].str;
        std::vector<size_t> bound_list;
        if (child[1].child.size() == 1)
            bound_list.push_back((size_t)child[1].child[0].value);
        else
            std::cout << "wrong at IR_SRef!" << std::endl;
        
        std::vector<Expr> index_list;
        
        ep = Var::make(data_type, sname, index_list, bound_list);
        
        if (sname == example.outs[0])
        {
            if (outputs.find(sname) == outputs.end())
                outputs[sname] = ep;
        }
        else
        {
            if (inputs.find(sname) == inputs.end())
                inputs[sname] = ep;
        }
        //HJH
        for(int i=0;i<grad_to.size();++i){
            if(compare_str(sname, grad_to[i])){
                grad_to_expr[sname]=ep;
                break;
            }
        }
    }

    // 处理张量
    void IR_TRef()
    {
        std::string tname = child[0].str;

        // 遍历Clist中的值，放入bound_list中
        std::vector<size_t> bound_list;
        for (int i = 0; i < child[1].child.size(); ++i)
        {
            //std::cout << child[1].child[i].value << " " ;
            bound_list.push_back((size_t)child[1].child[i].value);
        }

        //遍历Alist中的下标，放入index_list中
        std::vector<Expr> index_list;
        for (int i = 0; i < child[2].child.size(); ++i)
        {
            //std::cout << child[2].child[i].str << " "  ;
            index_list.push_back(child[2].child[i].ep);
        }

        // 定义出张量
        ep = Var::make(data_type, tname, index_list, bound_list);

        // 把本节点做bandcheck所需要的ep和value放入节点的bandcheck_list和bandcheck_value中
        std::string index_name;
        int index_value;
        for (int i = 0; i < child[1].child.size(); ++i)
        {
            // std::cout << child[1].child[i].value << " " ;
            index_name = child[2].child[i].str;
            index_value = child[1].child[i].value;

            // 如果对于同样的下标有更小的范围，则替换原来的value。
            if (bandcheck_list.find(index_name) != bandcheck_list.end())
            {
                if (bandcheck_value[index_name] > index_value)
                {
                    this->bandcheck_value[index_name] = child[1].child[i].value;
                    this->bandcheck_list[index_name] = child[2].child[i].ep;
                }
            }
            else
            {
                this->bandcheck_value[index_name] = child[1].child[i].value;
                this->bandcheck_list[index_name] = child[2].child[i].ep;
            }
        }

        // 处理out的代码
        if (compare_str(tname, example.outs[0]))
        {
            if (outputs.find(tname) == outputs.end())
                outputs[tname] = ep;
        }
        else
        {
            if (inputs.find(tname) == inputs.end())
                inputs[tname] = ep;
        }
        //HJH
        for(int i=0;i<grad_to.size();++i){
            if(compare_str(tname, grad_to[i])){
                grad_to_expr[tname]=ep;
                break;
            }
        }
    }

    void IR_CList()
    {
    }

    void IR_AList()
    {
    }

    void IR_IdExpr()
    {
        Expr dom_inf = Dom::make(index_type, -INF, 2 * INF); // 默认的下标的范围
        
        // 对于有儿子的节点，本节点的ep就是把子节点的ep合起来。
        if (child.size())
        {
            Expr ch1 = Expr(child[0].ep);
            Expr ch2 = Expr(child[2].ep);
            if (child[1].str == "+")
                ep = Binary::make(data_type, BinaryOpType::Add, ch1, ch2);
            else if (child[1].str == "-")
                ep = Binary::make(data_type, BinaryOpType::Sub, ch1, ch2);
            else if (child[1].str == "*")
                ep = Binary::make(data_type, BinaryOpType::Mul, ch1, ch2);
            else if (child[1].str == "/" || child[1].str == "//")
                ep = Binary::make(data_type, BinaryOpType::Div, ch1, ch2);
            else if (child[1].str == "%")
                ep = Binary::make(data_type, BinaryOpType::Mod, ch1, ch2);
            else
                std::cout << "Not Implemented Op in IdExpr!" << std::endl;
            
        }
        else
        {
            // 如果是下标的表达树的树叶，
            // 如果这个下标不是表达式，那我们根据CList中的值来缩小下标的循环范围。
            if (father && father->father)
            {
                if (father->t == 8)
                {
                    int pos = -1, upbound = -1;
                    for (int i = 0; i < father->child.size(); ++i)
                    {
                        if (father->child[i].str == str)
                        {
                            pos = i;
                            break;
                        }
                    }
                    if (pos != -1)
                    {
                        upbound = father->father->child[1].child[pos].value;
                        std::cout << "fuck " << str << " " << upbound << std::endl;
                        dom_inf = Dom::make(index_type, 0, upbound);
                    }
                }
            }
            ep = Index::make(index_type, str, dom_inf, IndexType::Spatial);
            
            // 把搜索到的下标放入全局变量，在IR_S中用于构造loop
            if (loop_indexs.find(str) == loop_indexs.end())
                loop_indexs[str] = ep;
        }
    }

    // 处理常数的值
    void IR_Const()
    {
        int xsd = str.find(".");
        int zhengshu = -1, xiaoshu = -1;
        if (xsd == -1)
        {
            zhengshu = atoi(str.c_str());
            //std::cout << zhengshu << std::endl;
            value = zhengshu;
            ep = Expr(zhengshu);
        }
        else
        {
            zhengshu = atoi(str.substr(0, xsd).c_str());
            int xiaoshulen = str.size() - xsd - 1;
            xiaoshu = atoi(str.substr(xsd + 1, xiaoshulen).c_str());
            float result = zhengshu + (xiaoshu / pow(10, xiaoshulen));
            //std::cout << result << std::endl;
            value = result;
            ep = Expr(result);
        }
    }
};

// todo: 从右边值节点开始递归求微分。
Expr find_dx(AST *RHS, int p_or_n)
{
    
}

int main(int argc, char *argv[])
{
    //HJH 
    //std::string src = "./cases/examples.json"; 
    std::string src = "./cases/case1.json";
    example = parse_json(src);
    
    example.print();

    AST root = AST((nodetype)0, example.kernel);
    root.build_tree();
    std::cout << "---build tree finish---" << std::endl;
    root.travel();
    std::cout << "***travel finish***" << std::endl;
    root.build_IR();
    std::cout << "***build IR finish***" << std::endl;
    std::cout << root.str << std::endl;

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
    return 0;

    // std::string src, dst;
    // for (int i = 0; i <= 10; ++i)
    // {
    //     if (i == 0)
    //         src = "./cases/example.json";
    //     else
    //         src = "./cases/case" + std::to_string(i) + ".json";
    //     if (access(src.c_str(), 0) == -1)
    //         continue;
    //     example = parse_json(src);
    //     example.print();

    //     dst = "./kernels/" + example.name + ".cc";
    //     if (example.data_type == "float")
    //         data_type = Type::float_scalar(32);
    //     else if (example.data_type == "int")
    //         data_type = Type::int_scalar(32);
    //     else
    //         std::cout << "data type in json not int or float!" << std::endl;

    //     AST root = AST((nodetype)0, example.kernel);
    //     root.build_tree();
    //     std::cout << "-----" << std::endl;
    //     root.travel();
    //     std::cout << "*****" << std::endl;
    //     root.build_IR();

    //     std::cout << root.str << std::endl;

    //     Group kernel = root.gp;

    //     // visitor
    //     IRVisitor visitor;
    //     root.gp.visit_group(&visitor);

    //     // mutator
    //     //IRMutator mutator;
    //     //kernel = mutator.mutate(kernel);

    //     // printer
    //     CCPrinter printer;
    //     std::string code = printer.print(root.gp);
    //     std::ofstream ofile(dst, std::ios::out);
    //     ofile << code;
    //     ofile.close();
    //     std::cout << "************" << std::endl;
    // }
}