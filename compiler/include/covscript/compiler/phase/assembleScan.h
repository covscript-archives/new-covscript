//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/compiler/compiler.h>

namespace cs {
    namespace compiler {
        class PhaseAssembleScan : public CompilerPhase {
        protected:
            void preparePhase(CompilerData &compilerData) override;

            void runPhase(CompilerData &compilerData,
                          CovScriptParser::CompilationUnitContext *compilationUnit) override;

            void postPhase(CompilerData &compilerData) override;

        public:
            PhaseAssembleScan();

            ~PhaseAssembleScan() override = default;
        };
    }
}
