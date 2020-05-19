//
// Created by hjh on 5/11/20.
//

#include "CCPrinter.h"


namespace Boost {

    namespace Internal {


        std::string CCPrinter::print(const Expr &expr) {
            oss.clear();
            expr.visit_expr(this);
            return oss.str();
        }


        std::string CCPrinter::print(const Stmt &stmt) {
            oss.clear();
            stmt.visit_stmt(this);
            return oss.str();
        }


        std::string CCPrinter::print(const Group &group) {
            oss.clear();
            group.visit_group(this);
            return oss.str();
        }


        void CCPrinter::visit(Ref<const IntImm> op) {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }


        void CCPrinter::visit(Ref<const UIntImm> op) {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }


        void CCPrinter::visit(Ref<const FloatImm> op) {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }

        // never used in hw1
        void CCPrinter::visit(Ref<const StringImm> op) {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }


        void CCPrinter::visit(Ref<const Unary> op) {
            if (op->op_type == UnaryOpType::Neg) {
                oss << "-";
            } else if (op->op_type == UnaryOpType::Not) {
                oss << "!";
            }
            (op->a).visit_expr(this);
        }


        void CCPrinter::visit(Ref<const Binary> op) {
            oss << "(";
            if(op->op_type == BinaryOpType::Mod) oss << "(int)";
            (op->a).visit_expr(this);
            if (op->op_type == BinaryOpType::Add) {
                oss << " + ";
            } else if (op->op_type == BinaryOpType::Sub) {
                oss << " - ";
            } else if (op->op_type == BinaryOpType::Mul) {
                oss << " * ";
            } else if (op->op_type == BinaryOpType::Div) {
                oss << " / ";
            } else if (op->op_type == BinaryOpType::Mod) {
                oss << " % ";
            } else if (op->op_type == BinaryOpType::And) {
                oss << " && ";
            } else if (op->op_type == BinaryOpType::Or) {
                oss << " || ";
            }
            (op->b).visit_expr(this);
            oss << ")";
        }


        void CCPrinter::visit(Ref<const Compare> op) {
            (op->a).visit_expr(this);
            if (op->op_type == CompareOpType::LT) {
                oss << " < ";
            } else if (op->op_type == CompareOpType::LE) {
                oss << " <= ";
            } else if (op->op_type == CompareOpType::EQ) {
                oss << " == ";
            } else if (op->op_type == CompareOpType::GE) {
                oss << " >= ";
            } else if (op->op_type == CompareOpType::GT) {
                oss << " > ";
            } else if (op->op_type == CompareOpType::NE) {
                oss << " != ";
            }
            (op->b).visit_expr(this);
        }


        void CCPrinter::visit(Ref<const Select> op) {
            oss << "((";
            (op->cond).visit_expr(this);
            oss << ") ? ";
            (op->true_value).visit_expr(this);
            oss << " : ";
            (op->false_value).visit_expr(this);
            oss << ")";
        }


        void CCPrinter::visit(Ref<const Call> op) {
            oss << op->func_name << "(";
            for (size_t i = 0; i < op->args.size(); ++i) {
                oss << ", ";
                op->args[i].visit_expr(this);
            }
            oss << ")";
        }


        void CCPrinter::visit(Ref<const Cast> op) {
            oss << "(" << op->new_type << ")(";
            (op->val).visit_expr(this);
            oss << ")";
        }

        //never used in hw1
        void CCPrinter::visit(Ref<const Ramp> op) {
            oss << "ramp(";
            (op->base).visit_expr(this);
            oss << ", " << op->stride << ", " << op->lanes << ")";
        }


        void CCPrinter::visit(Ref<const Var> op) {
            if (print_arg) {
                if(op->type().is_int()){
                    oss<<"int";
                }
                else{
                    oss<<"float";
                }
                oss <<" (&" << op->name <<")";
                if(op->args.empty()==false){
                    for (size_t i = 0; i < op->shape.size(); ++i) {
                        oss <<"[" << op->shape[i] << "]";
                    }
                }
            } else {
                oss << op->name;
                for (size_t i = 0; i < op->args.size(); ++i) {
                    oss << "[";
                    op->args[i].visit_expr(this);
                    oss << "]";
                }

            }
        }

        //never used in hw1
        void CCPrinter::visit(Ref<const Dom> op) {
            oss << "dom[";
            (op->begin).visit_expr(this);
            oss << ", ";
            (op->extent).visit_expr(this);
            oss << ")";
        }


        void CCPrinter::visit(Ref<const Index> op) {

            if (print_range) {
                if(op->type().is_int()){
                    oss<<"int";
                }
                else{
                    oss<<"float";
                }
                oss <<" " << op->name <<" = ";
                static_cast<const Dom*>((op->dom).get())->begin.visit_expr(this);
                oss <<"; " << op->name <<" < ";
                static_cast<const Dom*>((op->dom).get())->begin.visit_expr(this);
                oss << " + ";
                static_cast<const Dom*>((op->dom).get())->extent.visit_expr(this);
                oss << "; ++" << op->name;
            }
            else{
                oss << op->name;
            }
        }


        void CCPrinter::visit(Ref<const LoopNest> op) {
            print_range = true;
            for (auto index : op->index_list) {
                print_indent();
                oss << "for(";
                index.visit_expr(this);
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
                oss << "}\n";
            }
        }


        void CCPrinter::visit(Ref<const IfThenElse> op) {
            print_indent();
            oss << "if (";
            (op->cond).visit_expr(this);
            oss << ") {\n";
            enter();
            (op->true_case).visit_stmt(this);
            exit();
            print_indent();
            oss << "} else {\n";
            enter();
            (op->false_case).visit_stmt(this);
            exit();
            print_indent();
            oss << "}\n";
        }


        void CCPrinter::visit(Ref<const Move> op) {
            print_indent();
            (op->dst).visit_expr(this);
            oss << " =";
            (op->src).visit_expr(this);
            oss << ";\n";
        }


        void CCPrinter::visit(Ref<const Kernel> op) {
            print_indent();
            oss << "#include \"../run.h\"\n";
            oss << "void " << op->name << "(";
            arglist(op);
            oss << ") {\n";
            enter();
            declaretemp(op);

            for (auto stmt : op->stmt_list) {
                stmt.visit_stmt(this);
            }
            exit();
            oss << "}\n";
        }

        void CCPrinter::arglist(Ref<const Kernel> op){
            print_arg = true;
            std::vector<std::string> inputnames;
            for (size_t i = 0; i < op->inputs.size(); ++i) {
                op->inputs[i].visit_expr(this);
                if (i < op->inputs.size() - 1) {
                    oss << ", ";
                }
                inputnames.push_back(static_cast<const Var*>((op->inputs[i]).get())->name);
            }
            for (size_t i = 0; i < op->outputs.size(); ++i) {
                std::string name = static_cast<const Var*>((op->outputs[i]).get())->name;
                if(find(inputnames.begin(), inputnames.end(), name) != inputnames.end()) continue;
                if(i>0 || (i==0 && op->inputs.size()>0)){
                    oss << ", ";
                }
                op->outputs[i].visit_expr(this);
            }
            print_arg = false;
        }


        void CCPrinter::declaretemp(Ref<const Kernel> op){
            print_indent();
            //note!!
            //premise: outputs isn't empty
            if(op->outputs.size()==0){
                std::cout << "CCPrinter error: output is empty!!!"<<std::endl;
                return;
            }
            const Var* output =
                    static_cast<const Var*>((op->outputs[0]).get());
            if(output->type().is_int()){
                oss << "int";
            }
            else{
                oss << "float";
            }
            oss << " temp";
            for (size_t i = 0; i < output->shape.size(); ++i) {
                oss <<"[" << output->shape[i] << "]";
            }
            oss << " = {};\n";
        }

    }  // namespace Internal




}  // namespace Boost
