//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/compiler/scope.h>
#include <covscript/compiler/parser.h>
#include <list>

namespace cs {
    namespace compiler {
        class CompilerData {
            friend class CompilerPhase;

        private:
            Scope *_globalScope;

            Scope *getGlobalScope() { return _globalScope; }
        };

        class CompilerPhase {
        private:
            VMString _phaseName;

        protected:
            explicit CompilerPhase(VMString _phaseName);

            const VMString &getPhaseName() {
                return _phaseName;
            }

            virtual void preparePhase(CompilerData &compilerData) = 0;

            virtual void runPhase(CompilerData &compilerData,
                                  CovScriptParser::CompilationUnitContext *compilationUnit) = 0;

            virtual void postPhase(CompilerData &compilerData) = 0;
        };

        class Compiler {
        private:
            CompilerData _privateData;
            std::list<CompilerPhase *> _compilerPhases;
        };
    }
}
