#include <iostream>

#include <cstdio>
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
        try {
            std::rethrow_exception(e);
        } catch (antlr4::RecognitionException &re) {
            throw cs::compiler::SyntaxError(re.getCtx(), offendingSymbol, line, charPositionInLine, msg);
        }
    }
};

int main() {
    MyErrorHandler errorHandler;

    Parser parser("var $ = ");
    parser.removeErrorListeners();
    parser.getLexer().removeErrorListeners();
    parser.addErrorListener(&errorHandler);
    parser.getLexer().addErrorListener(&errorHandler);

    try {
        auto compilationUnit = parser.compilationUnit();
        MyVisitor visitor;
        visitor.visit(compilationUnit);

    } catch (SyntaxError &e) {
        printf("Syntax error at file: %s:%zd:%zd: %s\n",
            e.getOffendingSymbol()->getTokenSource()->getSourceName().c_str(),
            e.getLine(), e.getCharPosition(),
            e.getMessage().c_str());
        printf("%s\n\n", e.getRuleContext()->getText().c_str());
    }
    return 0;
}
