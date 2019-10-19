//
// Created by kiva on 2019/9/20.
//
#define CS_COMPILER_PHASE_IMPL

#include <covscript/compiler/phase/assembleDefine.hpp>

namespace cs {
    namespace compiler {
        PhaseAssembleDefine::PhaseAssembleDefine()
            : CompilerPhase("Assemble:Define") {
        }

        void PhaseAssembleDefine::preparePhase(CompilerData &compilerData) {
        }

        void PhaseAssembleDefine::runPhase(CompilerData &compilerData,
                                           CovScriptParser::CompilationUnitContext *compilationUnit) {
        }

        void PhaseAssembleDefine::postPhase(CompilerData &compilerData) {
        }
    }
}
