#include <iostream>

#include <cstdio>
#include <covscript/compiler/parser.h>
#include <CovScriptBaseVisitor.h>

using namespace cs::compiler;

class MyVisitor : public CovScriptBaseVisitor {
public:
    antlrcpp::Any visitVariableDeclStatement(CovScriptParser::VariableDeclStatementContext *ctx) override {
        if (ctx->IDENTIFIER() != nullptr) {
            printf("declare variable: %s\n", ctx->IDENTIFIER()->getText().c_str());

        } else if (ctx->variableBindingList() != nullptr) {
            printf("declare variables: ");
            auto list = ctx->variableBindingList();
            for (auto id : list->IDENTIFIER()) {
                printf("%s, ", id->getText().c_str());
            }
            printf("\n");
        }
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
    Parser parser(SourceFile("compiler/tests/csbuild.csc4"));
    MyErrorHandler errorHandler;

    parser.removeErrorListeners();
    parser.getLexer().removeErrorListeners();
    parser.addErrorListener(&errorHandler);
    parser.getLexer().addErrorListener(&errorHandler);

    try {
        auto compilationUnit = parser.compilationUnit();
        MyVisitor visitor;
        visitor.visit(compilationUnit);

    } catch (SyntaxError &e) {
        parser.printSyntaxError(e);
    }

    return 0;
}
