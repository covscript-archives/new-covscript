#include <covscript/compiler/compiler.hpp>
#include <covscript/compiler/phase/assembleDefine.hpp>
#include <covscript/compiler/phase/assembleScan.hpp>

using namespace cs::compiler;

int main() {
    CovScriptCompiler compiler;
    compiler.registerPhase<PhaseAssembleScan>();
    compiler.registerPhase<PhaseAssembleDefine>();

    compiler.addFile(makePtr<CodeSourceFile>("<code>", "var a = hello"));
    compiler.addFile(makePtr<RegularSourceFile>("compiler/tests/syntaxError.csc4"));

    try {
        compiler.compile();
    } catch (SyntaxError &e) {
        std::cerr << CovScriptCompiler::formatSyntaxError(e);
    }
    return 0;
}
