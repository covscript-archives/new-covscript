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
            printf("[Assemble:Define] Enter\n");
        }

        void PhaseAssembleDefine::runPhase(CompilerData &compilerData,
                                           CovScriptParser::CompilationUnitContext *compilationUnit) {
            printf("[Assemble:Define] Run with %p\n", compilationUnit);
        }

        void PhaseAssembleDefine::postPhase(CompilerData &compilerData) {
            printf("[Assemble:Define] Exit\n");
        }
    }
}
