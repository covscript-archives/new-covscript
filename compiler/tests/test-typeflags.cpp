//
// Created by kiva on 2019/9/24.
//
#include <covscript/compiler/variable.h>

int main() {
    using namespace cs::compiler;
    TypeFlag flags = TypeFlags::TYPE_UNKNOWN;

    // The type bool
    TypeFlags::setFlag(flags, TypeFlags::BOOL);
    // is primitive
    assert(TypeFlags::hasFlag(flags, TypeFlags::PRIMITIVE));
    // is not object
    assert(!TypeFlags::hasFlag(flags, TypeFlags::OBJECT));
    // is not int
    assert(!TypeFlags::hasFlag(flags, TypeFlags::INT));
    // is not float
    assert(!TypeFlags::hasFlag(flags, TypeFlags::FLOAT));
    // is not string
    assert(!TypeFlags::hasFlag(flags, TypeFlags::STRING));


    // The Type int
    TypeFlags::setFlag(flags, TypeFlags::INT);
    // is not float
    assert(!TypeFlags::hasFlag(flags, TypeFlags::FLOAT));
    // is not bool
    assert(!TypeFlags::hasFlag(flags, TypeFlags::BOOL));
    // is not char
    assert(!TypeFlags::hasFlag(flags, TypeFlags::CHAR));

    // The Type "un-primitive" int
    TypeFlags::setFlag(flags, TypeFlags::INT);
    TypeFlags::clearFlag(flags, TypeFlags::PRIMITIVE);
    // is not int
    assert(!TypeFlags::hasFlag(flags, TypeFlags::INT));

    // The Type lambda
    TypeFlags::setFlag(flags, TypeFlags::LAMBDA);
    // is object
    assert(TypeFlags::hasFlag(flags, TypeFlags::OBJECT));
    // is callable
    assert(TypeFlags::hasFlag(flags, TypeFlags::CALLABLE));
    // is lambda
    assert(TypeFlags::hasFlag(flags, TypeFlags::LAMBDA));
    // is not function
    assert(!TypeFlags::hasFlag(flags, TypeFlags::FUNCTION));
    // is not string
    assert(!TypeFlags::hasFlag(flags, TypeFlags::STRING));
    // is not primitive
    assert(!TypeFlags::hasFlag(flags, TypeFlags::PRIMITIVE));

    ///////////////////////////////////////////////////////////

    assert(LambdaType().isLambdaType());
    assert(LambdaType().isCallableType());
    assert(LambdaType().isObjectType());
    assert(!LambdaType().isFunctionType());
    assert(!LambdaType().isPrimitiveType());

    assert(IntType().isNumericType());
    assert(IntType().isPrimitiveType());
    assert(!IntType().isFloatType());
    assert(!IntType().isBoolType());
    assert(!IntType().isCharType());
}
