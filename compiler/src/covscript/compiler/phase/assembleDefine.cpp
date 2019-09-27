//
// Created by kiva on 2019/9/20.
//

#define COVSCRIPT_SDK_DEBUG
#define CS_COMPILER_PHASE_IMPL

#include <covscript/compiler/phase/assembleDefine.hpp>

namespace cs {
    namespace compiler {
        PhaseAssembleDefine::PhaseAssembleDefine()
            : CompilerPhase("Assemble:Define") {
        }

        void PhaseAssembleDefine::preparePhase(CompilerData &compilerData) {
            PHASE_LOG("[Assemble:Define] Enter");
        }

        void PhaseAssembleDefine::runPhase(CompilerData &compilerData,
                                           CovScriptParser::CompilationUnitContext *compilationUnit) {
            PHASE_LOG("[Assemble:Define] Run");
        }

        void PhaseAssembleDefine::postPhase(CompilerData &compilerData) {
            PHASE_LOG("[Assemble:Define] Exit");
        }
    }
}
