//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/sharedTypes.hpp>
#include <covscript/compiler/parser.hpp>
#include <unordered_map>
#include <vector>

namespace cs {
    namespace compiler {
        using TypeFlag = unsigned int;

        class TypeFlags {
        public:
            TypeFlags() = delete;
            TypeFlags(const TypeFlags &) = delete;
            TypeFlags(TypeFlags &&) = delete;

            enum {
                TYPE_ANY        = 0x0000,
                TYPE_UNKNOWN    = 0xFFFF,

                PRIMITIVE       = 0x1000,
                NUMERIC         = 0x1100,
                INT             = 0x1101,
                FLOAT           = 0x1102,
                BOOL            = 0x1201,
                CHAR            = 0x1202,
                STRING          = 0x1204,

                CALLABLE        = 0x2100,
                FUNCTION        = 0x2101,
                LAMBDA          = 0x2102,
            };

            static bool hasFlag(TypeFlag typeFlag, TypeFlag check) {
                return (typeFlag & check) == check;
            }

            static void setFlag(TypeFlag &current, TypeFlag add) {
                current = add;
            }

            static void clearFlag(TypeFlag &current, TypeFlag clear) {
                current &= ~clear;
            }
        };

        class Type;

        ////////////////////// Primitive Types
        class PrimitiveType;

        class NumericType;

        class IntType;

        class FloatType;

        class BoolType;

        class CharType;

        class StringType;
        //////////////////////////////////////

        ////////////////////// Object Types
        class CallableType;

        class FunctionType;

        class LambdaType;
        //////////////////////////////////////

        class Type {
        private:
            TypeFlag _typeFlag;

        public:
            constexpr explicit Type(TypeFlag typeFlag)
                : _typeFlag(typeFlag) {
            }

            ~Type() = default;

            TypeFlag getTypeFlag() const {
                return _typeFlag;
            }

            bool isUnknownType() const {
                return getTypeFlag() == TypeFlags::TYPE_UNKNOWN;
            }

            bool isAnyType() const {
                return !isUnknownType();
            }

            bool isPrimitiveType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::PRIMITIVE);
            }

            bool isNumericType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::NUMERIC);
            }

            bool isIntType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::INT);
            }

            bool isFloatType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::FLOAT);
            }

            bool isBoolType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::BOOL);
            }

            bool isCharType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::CHAR);
            }

            bool isStringType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::STRING);
            }

            bool isCallableType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::CALLABLE);
            }

            bool isFunctionType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::FUNCTION);
            }

            bool isLambdaType() const {
                return TypeFlags::hasFlag(getTypeFlag(), TypeFlags::LAMBDA);
            }

            bool operator==(Type &&other) const {
                return getTypeFlag() == other.getTypeFlag();
            }

            bool operator==(const Type &other) const {
                return getTypeFlag() == other.getTypeFlag();
            }

            bool operator!=(Type &&other) const {
                return getTypeFlag() != other.getTypeFlag();
            }

            bool operator!=(const Type &other) const {
                return getTypeFlag() != other.getTypeFlag();
            }

            VMString toString() const;
        };

        class PrimitiveType : public Type {
        public:
            explicit PrimitiveType(TypeFlag typeFlag);
            ~PrimitiveType() = default;
        };

        class NumericType : public PrimitiveType {
        public:
            explicit NumericType(TypeFlag typeFlag);
            ~NumericType() = default;
        };

        class IntType : public NumericType {
        public:
            IntType();
            ~IntType() = default;
        };

        class FloatType : public NumericType {
        public:
            FloatType();
            ~FloatType() = default;
        };

        class StringType : public PrimitiveType {
        public:
            StringType();
            ~StringType() = default;
        };

        class CharType : public PrimitiveType {
        public:
            CharType();
            ~CharType() = default;
        };

        class BoolType : public PrimitiveType {
        public:
            BoolType();
            ~BoolType() = default;
        };

        class CallableType : public Type {
        private:
            Type _returnType;
            std::vector<Type> _paramTypes;

        public:
            explicit CallableType(TypeFlag typeFlag);
            ~CallableType() = default;

            Type getReturnType() const {
                return _returnType;
            }

            const std::vector<Type>& getParamTypes() const {
                return _paramTypes;
            }

            void setReturnType(Type type) {
                this->_returnType = type;
            }

            void setParamTypes(const std::vector<Type>& paramTypes) {
                this->_paramTypes = paramTypes;
            }
        };

        class FunctionType : public CallableType {
        public:
            FunctionType();
            ~FunctionType() = default;
        };

        class LambdaType : public CallableType {
        public:
            LambdaType();
            ~LambdaType() = default;
        };

        class Variable {
        private:
            VMString _variableName;
            Parser::ExpressionContext *_variableInit;
            bool _isLiteral;
            Type _variableType;

        protected:
            void setLiteral(bool isLiteral) { _isLiteral = isLiteral; }

        public:
            Variable(VMString variableName, Parser::ExpressionContext *variableInit);

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
