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

class MyErrorHandler : public antlr4::BaseErrorListener {
    void
    syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                const std::string &msg, std::exception_ptr e) override {

    }
};

int main() {
    MyErrorHandler errorHandler;

    Parser parser("var $ = 10086");
    parser.removeErrorListeners();
    parser.getLexer().removeErrorListeners();
    parser.addErrorListener(&errorHandler);
    parser.getLexer().addErrorListener(&errorHandler);

    auto compilationUnit = parser.compilationUnit();
    MyVisitor visitor;
    visitor.visit(compilationUnit);
    return 0;
}
