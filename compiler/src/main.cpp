#include <iostream>

#include <cstdio>
#include <covscript/compiler/compiler.hpp>
#include <covscript/compiler/phase/assembleDefine.hpp>
#include <covscript/compiler/phase/assembleScan.hpp>
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
