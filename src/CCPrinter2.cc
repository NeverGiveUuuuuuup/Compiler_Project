//
// Created by hjh on 5/11/20.
//

#include "CCPrinter2.h"


namespace Boost {

    namespace Internal {


        std::string CCPrinter2::print(const Expr &expr) {
            oss.clear();
            expr.visit_expr(this);
            return oss.str();
        }


        std::string CCPrinter2::print(const Stmt &stmt) {
            oss.clear();
            stmt.visit_stmt(this);
            return oss.str();
        }


        std::string CCPrinter2::print(const Group &group) {
            oss.clear();
            group.visit_group(this);
            return oss.str();
        }


        void CCPrinter2::visit(Ref<const IntImm> op) {
            oss<<op->value();
            std::cout << op->value();
        }


        void CCPrinter2::visit(Ref<const UIntImm> op) {
            oss<<op->value();
            std::cout << op->value();
        }


        void CCPrinter2::visit(Ref<const FloatImm> op) {
            oss<<op->value();
            std::cout << op->value();
        }

        // never used in hw1
        void CCPrinter2::visit(Ref<const StringImm> op) {
            oss<<op->value();
            std::cout << op->value();
        }


        void CCPrinter2::visit(Ref<const Unary> op) {
            if (op->op_type == UnaryOpType::Neg) {
                oss<< "-";
                std::cout << "-";
            } else if (op->op_type == UnaryOpType::Not) {
                oss<< "!";
                std::cout << "!";
            }
            (op->a).visit_expr(this);
        }


        void CCPrinter2::visit(Ref<const Binary> op) {
            std::cout << "(";
            oss<< "(";
            if(op->op_type == BinaryOpType::Mod){
                oss<< "(int)";
                std::cout << "(int)";
            }
            (op->a).visit_expr(this);
            if (op->op_type == BinaryOpType::Add) {
                std::cout << " + ";
                oss<< " + ";
            } else if (op->op_type == BinaryOpType::Sub) {
                std::cout << " - ";
                oss<< " - ";
            } else if (op->op_type == BinaryOpType::Mul) {
                std::cout << " * ";
                oss<< " * ";
            } else if (op->op_type == BinaryOpType::Div) {
                std::cout << " / ";
                oss<< " / ";
            } else if (op->op_type == BinaryOpType::Mod) {
                std::cout << " % ";
                oss<< " % ";
            } else if (op->op_type == BinaryOpType::And) {
                std::cout << " && ";
                oss<< " && ";
            } else if (op->op_type == BinaryOpType::Or) {
                std::cout <<  " || ";
                oss<< " || ";
            }
            (op->b).visit_expr(this);
            std::cout << ")";
            oss<< ")";
        }


        void CCPrinter2::visit(Ref<const Compare> op) {
            (op->a).visit_expr(this);
            if (op->op_type == CompareOpType::LT) {
                std::cout << " < ";
                oss << " < ";
                
            } else if (op->op_type == CompareOpType::LE) {
                std::cout << " <= ";
                oss << " <= ";
            } else if (op->op_type == CompareOpType::EQ) {
                std::cout << " == ";
                oss << " == ";
            } else if (op->op_type == CompareOpType::GE) {
                std::cout << " >= ";
                oss << " >= ";
            } else if (op->op_type == CompareOpType::GT) {
                std::cout << " > ";
                oss << " > ";
            } else if (op->op_type == CompareOpType::NE) {
                std::cout << " != ";
                oss << " != ";
            }
            (op->b).visit_expr(this);
        }


        void CCPrinter2::visit(Ref<const Select> op) {
            std::cout << "((";
            oss << "((";
            (op->cond).visit_expr(this);
            std::cout << ") ? ";
            oss << ") ? ";
            (op->true_value).visit_expr(this);
            std::cout << " : ";
            oss << " : ";
            (op->false_value).visit_expr(this);
            std::cout << ")";
            oss << ")";
        }


        void CCPrinter2::visit(Ref<const Call> op) {
            std::cout << op->func_name << "(";
            oss << op->func_name << "(";
            for (size_t i = 0; i < op->args.size(); ++i) {
                std::cout << ", ";
                oss << ", ";
                op->args[i].visit_expr(this);
            }
            std::cout << ")";
            oss << ")";
        }


        void CCPrinter2::visit(Ref<const Cast> op) {
            std::cout << "(" << op->new_type << ")(";
            oss << "(" << op->new_type << ")(";
            (op->val).visit_expr(this);
            std::cout << ")";
            oss << ")";
        }

        //never used in hw1
        void CCPrinter2::visit(Ref<const Ramp> op) {
            //std::cout << "ramp(";
            (op->base).visit_expr(this);
            //std::cout << ", " << op->stride << ", " << op->lanes << ")";
        }


        void CCPrinter2::visit(Ref<const Var> op) {
            if (print_arg) {
                

                if(op->type().is_int()){
                    std::cout<<"int";
                    oss<<"int";
                }
                else{
                    std::cout<<"float";
                    oss<<"float";
                }
                std::cout <<" (&" << op->name <<")";
                oss <<" (&" << op->name <<")";
                if(op->args.empty()==false){
                    for (size_t i = 0; i < op->shape.size(); ++i) {
                        std::cout <<"[" << op->shape[i] << "]";
                        oss <<"[" << op->shape[i] << "]";
                    }
                }
            } else {
                std::cout << op->name;
                oss << op->name;
                for (size_t i = 0; i < op->args.size(); ++i) {
                    std::cout << "[";
                    oss << "[";
                    op->args[i].visit_expr(this);
                    std::cout << "]";
                    oss << "]";
                }

            }
        }

        //never used in hw1
        void CCPrinter2::visit(Ref<const Dom> op) {
            //std::cout << "dom[";
            (op->begin).visit_expr(this);
            //std::cout << ", ";
            (op->extent).visit_expr(this);
            //std::cout << ")";
        }


        void CCPrinter2::visit(Ref<const Index> op) {

            if (print_range) {
                if(op->type().is_int()){
                    std::cout<<"int";
                    oss<<"int";
                }
                else{
                    std::cout<<"float";
                    oss<<"float";
                }
                std::cout <<" " << op->name <<" = ";
                oss <<" " << op->name <<" = ";
                static_cast<const Dom*>((op->dom).get())->begin.visit_expr(this);
                std::cout <<"; " << op->name <<" < ";
                oss <<"; " << op->name <<" < ";
                static_cast<const Dom*>((op->dom).get())->begin.visit_expr(this);
                std::cout << " + ";
                oss << " + ";
                static_cast<const Dom*>((op->dom).get())->extent.visit_expr(this);
                std::cout << "; ++" << op->name;
                oss << "; ++" << op->name;
            }
            else{
                std::cout << op->name;
                oss << op->name;
            }
        }


        void CCPrinter2::visit(Ref<const LoopNest> op) {
            print_range = true;
            for (auto index : op->index_list) {
                print_indent();
                std::cout << "for(";
                oss << "for(";
                index.visit_expr(this);
                std::cout << "){\n";
                oss << "){\n";
                enter();
            }
            print_range = false;
            for (auto body : op->body_list) {
                body.visit_stmt(this);
            }
            for (auto index : op->index_list) {
                exit();
                print_indent();
                std::cout << "}\n";
                oss << "}\n";
            }
        }


        void CCPrinter2::visit(Ref<const IfThenElse> op) {
            print_indent();
            std::cout << "if (";
            oss << "if (";
            (op->cond).visit_expr(this);
            std::cout << ") {\n";
            oss << ") {\n";
            enter();
            (op->true_case).visit_stmt(this);
            exit();
            print_indent();
            std::cout << "} else {\n";
            oss << "} else {\n";
            enter();
            (op->false_case).visit_stmt(this);
            exit();
            print_indent();
            std::cout << "}\n";
            oss << "}\n";
        }


        void CCPrinter2::visit(Ref<const Move> op) {
            print_indent();
            (op->dst).visit_expr(this);
            std::cout << " =";
            oss << " =";
            (op->src).visit_expr(this);
            std::cout << ";\n";
            oss << ";\n";
        }


        void CCPrinter2::visit(Ref<const Kernel> op) {
            std::cout<<"开始生成代码"<<std::endl;
        
            print_indent();
            
            std::cout << "#include \"../run.h\"\n";
            oss << "#include \"../run.h\"\n";
            std::cout << "void " << op->name << "(";
            oss << "void " << op->name << "(";
            arglist(op);
            std::cout << ") {\n";
            oss << ") {\n";
            enter();
            declaretemp(op);
            
            for (auto stmt : op->stmt_list) {
                stmt.visit_stmt(this);
            }
            exit();
            std::cout << "}\n";
            oss << "}\n";
            std::cout<<"完成代码生成"<<std::endl;
        }

        void CCPrinter2::arglist(Ref<const Kernel> op){
            std::cout<<"开始生成参数"<<std::endl;
        
            print_arg = true;
            std::vector<std::string> inputnames;
            for (size_t i = 0; i < op->inputs.size(); ++i) {
                std::cout<<"打印参数";
                /*
                if(op->inputs[i].defined()==false){
                    std::cout<<"参数未定义";
                    continue;
                }
                */
                op->inputs[i].visit_expr(this);
                if (i < op->inputs.size() - 1) {
                    std::cout << ", ";
                    oss << ", ";
                }
                inputnames.push_back(static_cast<const Var*>((op->inputs[i]).get())->name);
            }
            
            for (size_t i = 0; i < op->outputs.size(); ++i) {
                std::string name = static_cast<const Var*>((op->outputs[i]).get())->name;
                if(find(inputnames.begin(), inputnames.end(), name) != inputnames.end()) continue;
                if(i>0 || (i==0 && op->inputs.size()>0)){
                    std::cout << ", ";
                    oss << ", ";
                }
                op->outputs[i].visit_expr(this);
            }
            print_arg = false;
            std::cout<<"完成生成参数"<<std::endl;
        
        }


        void CCPrinter2::declaretemp(Ref<const Kernel> op){
            print_indent();
            //note!!
            //premise: outputs isn't empty
            if(op->outputs.size()==0){
                std::cout << "CCPrinter2: output is empty!!!"<<std::endl;
                return;
            }
            const Var* output =
                    static_cast<const Var*>((op->outputs[0]).get());
            if(output->type().is_int()){
                std::cout << "int";
                oss << "int";
            }
            else{
                std::cout << "float";
                oss << "float";
            }
            std::cout << " temp";
            oss << " temp";
            for (size_t i = 0; i < output->shape.size(); ++i) {
                std::cout <<"[" << output->shape[i] << "]";
                oss <<"[" << output->shape[i] << "]";
            }
            std::cout << " = {};\n";
            oss << " = {};\n";
        }

    }  // namespace Internal




}  // namespace Boost
