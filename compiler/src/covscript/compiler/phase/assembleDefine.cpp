//
// Created by kiva on 2019/9/20.
//

#include <covscript/compiler/phase/assembleDefine.h>

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
