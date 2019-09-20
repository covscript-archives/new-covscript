//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/sharedTypes.h>

namespace cs {
    namespace compiler {
        class Type;

        class PrimitiveType;

        class NumericType;

        class IntType;

        class FloatType;

        class BoolType;

        class CharType;

        class StringType;

        class ClassType;

        class CallableType;

        class FunctionType;

        class LambdaType;

        class Type {
        public:
            bool isPrimitiveType() const {
                return is<const PrimitiveType *>(this);
            }

            bool isNumericType() const {
                return is<const NumericType *>(this);
            }

            bool isIntType() const {
                return is<const IntType *>(this);
            }

            bool isFloatType() const {
                return is<const FloatType *>(this);
            }

            bool isBoolType() const {
                return is<const BoolType *>(this);
            }

            bool isCharType() const {
                return is<const CharType *>(this);
            }

            bool isStringType() const {
                return is<const StringType *>(this);
            }

            bool isObjectType() const {
                return is<const ClassType *>(this);
            }

            bool isCallableType() const {
                return is<const CallableType *>(this);
            }

            bool isFunctionType() const {
                return is<const FunctionType *>(this);
            }

            bool isLambdaType() const {
                return is<const LambdaType *>(this);
            }

            Type() = default;

            virtual ~Type() = default;
        };

        class PrimitiveType : public Type {
        };

        class NumericType : public PrimitiveType {
        };

        class IntType : public NumericType {
        };

        class FloatType : public NumericType {
        };

        class StringType : public PrimitiveType {
        };

        class CharType : public PrimitiveType {
        };

        class BoolType : public PrimitiveType {
        };

        class ClassType : public Type {
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
            Type _variableType;

        protected:
            void setLiteral(bool isLiteral) { _isLiteral = isLiteral; }

        public:
            bool isLiteral() const { return _isLiteral; }

            const VMString &getName() const { return _variableName; }

            const Type &getType() const { return _variableType; }

            bool isPrimitiveType() const {
                return _variableType.isPrimitiveType();
            }

            bool isNumericType() const {
                return _variableType.isNumericType();
            }

            bool isIntType() const {
                return _variableType.isIntType();
            }

            bool isFloatType() const {
                return _variableType.isFloatType();
            }

            bool isBoolType() const {
                return _variableType.isBoolType();
            }

            bool isCharType() const {
                return _variableType.isCharType();
            }

            bool isStringType() const {
                return _variableType.isStringType();
            }

            bool isObjectType() const {
                return _variableType.isObjectType();
            }

            bool isCallableType() const {
                return _variableType.isCallableType();
            }

            bool isFunctionType() const {
                return _variableType.isFunctionType();
            }

            bool isLambdaType() const {
                return _variableType.isLambdaType();
            }
        };
    }
}
