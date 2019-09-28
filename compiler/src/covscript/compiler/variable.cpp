//
// Created by kiva on 2019/9/24.
//

#include <covscript/compiler/variable.hpp>

namespace cs {
    namespace compiler {
        Type::Type(TypeFlag typeFlag)
            : _typeFlag(typeFlag) {
        }

        VMString Type::toString() const {
            switch (getTypeFlag()) {
                case TypeFlags::TYPE_ANY:
                    return "any";
                case TypeFlags::INT:
                    return "int";
                case TypeFlags::FLOAT:
                    return "float";
                case TypeFlags::STRING:
                    return "string";
                case TypeFlags::CHAR:
                    return "char";
                case TypeFlags::BOOL:
                    return "bool";
                case TypeFlags::FUNCTION:
                    return "function";
                case TypeFlags::LAMBDA:
                    return "lambda";
                case TypeFlags::OBJECT:
                    return "object";
                default:
                    return "<unknown>";
            }
        }

        PrimitiveType::PrimitiveType(TypeFlag typeFlag)
            : Type(typeFlag) {
        }

        NumericType::NumericType(TypeFlag typeFlag)
            : PrimitiveType(typeFlag) {
        }

        CallableType::CallableType(TypeFlag typeFlag)
            : Type(typeFlag) {
        }

        IntType::IntType()
            : NumericType(TypeFlags::INT) {
        }

        FloatType::FloatType()
            : NumericType(TypeFlags::FLOAT) {
        }

        StringType::StringType()
            : PrimitiveType(TypeFlags::STRING) {
        }

        CharType::CharType()
            : PrimitiveType(TypeFlags::CHAR) {
        }

        BoolType::BoolType()
            : PrimitiveType(TypeFlags::BOOL) {
        }

        ObjectType::ObjectType()
            : Type(TypeFlags::OBJECT) {
        }

        FunctionType::FunctionType()
            : CallableType(TypeFlags::CALLABLE) {
        }

        LambdaType::LambdaType()
            : CallableType(TypeFlags::LAMBDA) {
        }
    }
}
