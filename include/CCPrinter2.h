//
// Created by hjh on 5/11/20.
//

#ifndef BOOST_CCPRINTER_H
#define BOOST_CCPRINTER_H

#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

#include "IRPrinter.h"
#include "IRVisitor.h"

namespace Boost {

    namespace Internal {

        class CCPrinter2 : public IRVisitor {
        public:
            CCPrinter2() : IRVisitor() {
                indent = 0;
                print_range = false;
                print_arg = false;
            }
            std::string print(const Expr&);
            std::string print(const Stmt&);
            std::string print(const Group&);

            void print_indent() {
                for (int i = 0; i < indent; ++i){
                    oss << " ";
                    std::cout << " ";
                }
                    
            }

            void enter() {
                indent += 2;
            }

            void exit() {
                indent -= 2;
            }
            void declaretemp(Ref<const Kernel> op);
            void arglist(Ref<const Kernel> op);

            void visit(Ref<const IntImm>) override;
            void visit(Ref<const UIntImm>) override;
            void visit(Ref<const FloatImm>) override;
            void visit(Ref<const StringImm>) override;
            void visit(Ref<const Unary>) override;
            void visit(Ref<const Binary>) override;
            void visit(Ref<const Select>) override;
            void visit(Ref<const Compare>) override;
            void visit(Ref<const Call>) override;
            void visit(Ref<const Var>) override;
            void visit(Ref<const Cast>) override;
            void visit(Ref<const Ramp>) override;
            void visit(Ref<const Index>) override;
            void visit(Ref<const Dom>) override;
            void visit(Ref<const LoopNest>) override;
            void visit(Ref<const IfThenElse>) override;
            void visit(Ref<const Move>) override;
            void visit(Ref<const Kernel>) override;
        private:
            std::ostringstream oss;
            int indent;
            bool print_range;
            bool print_arg;
        };

    }  // namespace Internal

}  // namespace Boost


#endif  // BOOST_IRPRINTER_H