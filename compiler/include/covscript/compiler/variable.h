//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/compilerTypes.h>

namespace cs {
    namespace compiler {
        class Type {
        };

        class PrimitiveType : public Type {
        };

        class NumericType : public PrimitiveType {
        };

        class IntVariable : public NumericType {
        };

        class FloatVariable : public NumericType {
        };

        class StringVariable : public PrimitiveType {
        };

        class CharVariable : public PrimitiveType {
        };

        class BoolVariable : public PrimitiveType {
        };

        class ObjectVariable : public Type {
        };

        class CallableType : public Type {
        };

        class FunctionType : public CallableType {
        };

        class LambdaType : public CallableType {
        };

        class Variable {
        private:
            VMString _variableName;
            bool _isLiteral;

        protected:
            void setLiteral(bool isLiteral) { _isLiteral = isLiteral; }

        public:
            bool isLiteral() const { return _isLiteral; }

            const VMString &getName() const { return _variableName; }
        };
    }
}
