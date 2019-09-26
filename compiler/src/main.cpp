#include <iostream>

#include <cstdio>
#include <covscript/compiler/compiler.h>
#include <covscript/compiler/phase/assembleDefine.h>
#include <covscript/compiler/phase/assembleScan.h>
#include <CovScriptBaseVisitor.h>

using namespace cs::compiler;

int main() {
    CovScriptCompiler compiler;
    compiler.registerPhase<PhaseAssembleScan>();
    compiler.registerPhase<PhaseAssembleDefine>();

    compiler.addFile(makePtr<CodeSourceFile>("<code>", "var a = hello"));
    compiler.addFile(makePtr<RegularSourceFile>("compiler/tests/hello.csc4"));
    compiler.compile();

    return 0;
}
