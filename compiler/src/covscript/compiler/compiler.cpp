//
// Created by kiva on 2019/9/20.
//
#include <covscript/compiler/compiler.h>

namespace cs {
    namespace compiler {
        CompilerPhase::CompilerPhase(VMString _phaseName)
            : _phaseName(std::move(_phaseName)) {

        }
    }
}
