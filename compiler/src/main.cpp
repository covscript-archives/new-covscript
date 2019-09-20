#include <iostream>

#include <cstdio>
#include <covscript/compiler/compiler.h>
#include <covscript/compiler/phase/assembleDefine.h>
#include <covscript/compiler/phase/assembleScan.h>
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

class CustomPhase : public CompilerPhase {
public:
    CustomPhase() : CompilerPhase("Verify:Debug") {}

    ~CustomPhase() override = default;

protected:
    void preparePhase(CompilerData &compilerData) override {
        printf("[Verify:Debug]: %p, prepare phase\n", this);
    }

    void runPhase(CompilerData &compilerData, CovScriptParser::CompilationUnitContext *compilationUnit) override {
        printf("[Verify:Debug]: %p, run phase with %p\n", this, compilationUnit);
        MyVisitor myVisitor;
        myVisitor.visitCompilationUnit(compilationUnit);
    }

    void postPhase(CompilerData &compilerData) override {
        printf("[Verify:Debug]: %p, post phase\n", this);
    }
};

int main() {
    auto sourceFile = makePtr<CodeSourceFile>("<hello>", "var s = 0");
    Parser parser(sourceFile);

    parser.removeErrorListeners();
    parser.getLexer().removeErrorListeners();

    try {
        auto compilationUnit = parser.compilationUnit();
        MyVisitor visitor;
        visitor.visitCompilationUnit(compilationUnit);

    } catch (SyntaxError &e) {
        parser.printSyntaxError(e);
    }

    //
    printf("WTF HERE\n");
    CovScriptCompiler compiler;
    compiler.registerPhase<PhaseAssembleScan>();
    compiler.registerPhase<PhaseAssembleDefine>();
    compiler.registerPhase<CustomPhase>();

    compiler.addFile(makePtr<CodeSourceFile>("<code>", "var a = hello"));
    compiler.addFile(makePtr<RegularSourceFile>("compiler/tests/hello.csc4"));
    compiler.compile();

    return 0;
}
