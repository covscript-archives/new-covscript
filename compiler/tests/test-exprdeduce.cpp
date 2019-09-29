//
// Created by kiva on 2019/9/29.
//

#include <covscript/compiler/typeDeduce.hpp>
#include <covscript/compiler/parser.hpp>

using namespace cs::compiler;

void deduce(Type &&expected, const char *code) {
    Parser parser(makePtr<CodeSourceFile>("<code>", code));
    auto expr = parser.expression();
    Type deduced = TypeDeduce::deduceExpr(expr);

    if (deduced != expected) {
        fprintf(stderr, "Type deduction failed on expression: (%s): "
                        "Deduced type = %s, expected type = %s\n",
            code, deduced.toString().c_str(), expected.toString().c_str());
    }
}

int main() {
    deduce(IntType(), "1");
    deduce(IntType(), "0");
    deduce(FloatType(), "0.0");
    deduce(StringType(), "1 + 2 * 3 + 4 / 4 + 1 + \"\"");
    deduce(IntType(), "'a' + 11");
    return 0;
}
