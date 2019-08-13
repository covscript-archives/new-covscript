#include <iostream>

#include <covscript/compiler/parser.h>
#include <CovScriptBaseVisitor.h>

using namespace cs::compiler;

class MyVisitor : public CovScriptBaseVisitor {
public:
    antlrcpp::Any visitVariableDeclStatement(CovScriptParser::VariableDeclStatementContext *ctx) override {
        printf("declare variable: %s", ctx->IDENTIFIER()->getText().c_str());
        return CovScriptBaseVisitor::visitVariableDeclStatement(ctx);
    }
};

int main() {
    Parser parser("var hello_covscript4 = 10086");
    auto compilationUnit = parser.compilationUnit();
    MyVisitor visitor;
    visitor.visit(compilationUnit);
    return 0;
}
