//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/types.h>

namespace cs {
    namespace compiler {
        class Variable {
        private:
            VMString variableName;

        public:
            const VMString &getName() const { return variableName; }
        };

        class StringVariable : public Variable {
        };

        class CharVariable : public Variable {
        };

        class BoolVariable : public Variable {
        };

        class ObjectVariable : public Variable {
        };

        class CallableVariable : public Variable {
        };

        class FunctionVariable : public CallableVariable {
        };

        class LambdaVariable : public CallableVariable {
        };
    }
}
