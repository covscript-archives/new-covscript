//
// Created by kiva on 2019/9/24.
//

#include <covscript/compiler/variable.h>

namespace cs {
    namespace compiler {
        Type::Type(TypeFlag typeFlag)
            : _typeFlag(typeFlag) {
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
