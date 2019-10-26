//
// Created by kiva on 2019/9/29.
//
#define CS_COMPILER_PHASE_IMPL

#include <covscript/compiler/typeDeduce.hpp>
#include <covscript/compiler/compiler.hpp>
#include <covscript/compiler/phase/assembleScan.hpp>
#include <covscript/compiler/phase/assembleDefine.hpp>

using namespace cs::compiler;

class DeduceVisitor : public CovScriptBaseVisitor {
public:
    antlrcpp::Any visitVariableDeclStatement(CovScriptParser::VariableDeclStatementContext *ctx) override {
        if (ctx->nameAndType() != nullptr) {
            printf("declare variable: %s", ctx->nameAndType()->IDENTIFIER()->getText().c_str());

        } else if (ctx->variableBindingList() != nullptr) {
            printf("declare variables: ");
            auto list = ctx->variableBindingList();
            for (auto id : list->nameAndType()) {
                printf("%s, ", id->IDENTIFIER()->getText().c_str());
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
        printf("[Verify:TypeDeduceTest]: Enter\n");
    }

    void runPhase(CompilerData &compilerData, CovScriptParser::CompilationUnitContext *compilationUnit) override {
        printf("[Verify:TypeDeduceTest]: Run with %p\n", compilationUnit);
        DeduceVisitor visitor;
        visitor.visit(compilationUnit);
    }

    void postPhase(CompilerData &compilerData) override {
        printf("[Verify:TypeDeduceTest]: Exit\n");
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
        "var promotion4 = 3 + 1 * 0.9\n"
        "var promotion5 = a.toString()\n"
        "var promotion6 = c.toFloat()\n"
        "var arr1 = {1, 2, 4}\n"
        "var map1 = (0:1, 1:2)\n"));

    compiler.setVerbose(true);
    compiler.registerPhase<PhaseAssembleScan>();
    compiler.registerPhase<PhaseAssembleDefine>();
    compiler.addPhase(makePtr<TypeDeduceTestPhase>());
    compiler.compile();
    return 0;
}
