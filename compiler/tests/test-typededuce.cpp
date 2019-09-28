//
// Created by kiva on 2019/9/29.
//
#define CS_COMPILER_PHASE_IMPL

#include <covscript/compiler/typeDeduce.hpp>
#include <covscript/compiler/compiler.hpp>

using namespace cs::compiler;

class DeduceVisitor : public CovScriptBaseVisitor {
public:
    antlrcpp::Any visitVariableDeclStatement(CovScriptParser::VariableDeclStatementContext *ctx) override {
        if (ctx->IDENTIFIER() != nullptr) {
            printf("declare variable: %s", ctx->IDENTIFIER()->getText().c_str());

        } else if (ctx->variableBindingList() != nullptr) {
            printf("declare variables: ");
            auto list = ctx->variableBindingList();
            for (auto id : list->IDENTIFIER()) {
                printf("%s, ", id->getText().c_str());
            }
        }

        printf(", with type: ");

        Type varType = TypeDeduce::deduceExpr(ctx->expression());
        printf("%s\n", varType.toString().c_str());

        return CovScriptBaseVisitor::visitVariableDeclStatement(ctx);
    }
};

class TypeDeduceTestPhase : public CompilerPhase {
public:
    TypeDeduceTestPhase() : CompilerPhase("Verify:TypeDeduceTest") {}

    ~TypeDeduceTestPhase() override = default;

protected:
    void preparePhase(CompilerData &compilerData) override {
    }

    void runPhase(CompilerData &compilerData, CovScriptParser::CompilationUnitContext *compilationUnit) override {
        DeduceVisitor visitor;
        visitor.visit(compilationUnit);
    }

    void postPhase(CompilerData &compilerData) override {

    }
};

int main() {
    CovScriptCompiler compiler;
    compiler.addFile(makePtr<CodeSourceFile>("<code>",
        "var a = hello\n"
        "var b = \"hello\"\n"
        "var c = 10\n"
        "var d = 10.0\n"
        "var e = 1 > 2\n"
        "var promotion1 = 1 + 1.0\n"
        "var promotion2 = 'c' + \"hello\"\n"
        "var promotion3 = 'c' + 'f'\n"
        "var promotion4 = 3 + 1 * 0.9"));

    compiler.registerPhase<TypeDeduceTestPhase>();
    compiler.compile();
    return 0;
}
