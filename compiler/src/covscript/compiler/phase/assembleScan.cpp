//
// Created by kiva on 2019/9/20.
//

#define COVSCRIPT_SDK_DEBUG
#define CS_COMPILER_PHASE_IMPL

#include <covscript/compiler/phase/assembleScan.hpp>

namespace cs {
    namespace compiler {
        PhaseAssembleScan::PhaseAssembleScan()
            : CompilerPhase("Assemble:Scan") {
        }

        void PhaseAssembleScan::preparePhase(CompilerData &compilerData) {
        }

        void PhaseAssembleScan::runPhase(CompilerData &compilerData,
                                         CovScriptParser::CompilationUnitContext *compilationUnit) {
        }

        void PhaseAssembleScan::postPhase(CompilerData &compilerData) {
        }
    }
}
