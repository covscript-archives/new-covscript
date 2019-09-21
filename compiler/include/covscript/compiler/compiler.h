//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/compiler/scope.h>
#include <covscript/compiler/parser.h>
#include <unordered_map>
#include <list>

namespace cs {
    namespace compiler {
        class CompilerData {
        public:
            using CompileFiles = std::unordered_map<Ptr<SourceFile>, Parser *>;

        private:
            Scope *_globalScope;
            CompileFiles _compileFiles;

        public:
            Scope *getGlobalScope() { return _globalScope; }

            CompileFiles &getCompileFiles() { return _compileFiles; }
        };

        class CompilerPhase {
            friend class BaseCompiler;

        private:
            /**
             * The phase name must satisfy the following rule:
             * <PhasePurpose>:<Routine>
             * where PhasePurpose is one of these:
             *  * Assemble
             *  * Optimize
             *  * CodeGen
             *  * Verify
             * Routine is unlimited.
             */
            VMString _phaseName;

        protected:
            explicit CompilerPhase(VMString _phaseName);

            virtual ~CompilerPhase() = default;

            const VMString &getPhaseName() {
                return _phaseName;
            }

            virtual void preparePhase(CompilerData &compilerData) = 0;

            virtual void runPhase(CompilerData &compilerData,
                                  CovScriptParser::CompilationUnitContext *compilationUnit) = 0;

            virtual void postPhase(CompilerData &compilerData) = 0;
        };

        class BaseCompiler {
        protected:
            CompilerData _privateData;
            std::list<Ptr<CompilerPhase>> _compilerPhases;

        protected:
            void preparePhase(Ptr<CompilerPhase> &phase);

            void runPhase(Ptr<CompilerPhase> &phase,
                          CovScriptParser::CompilationUnitContext *compilationUnit);

            void postPhase(Ptr<CompilerPhase> &phase);

        public:
            BaseCompiler() = default;

            virtual ~BaseCompiler() = default;

            template<typename T>
            void registerPhase() {
                _compilerPhases.push_back(makePtr<T>());
            }
        };

        class CompilerErrorHandler : public antlr4::BaseErrorListener {
            void
            syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                        size_t line, size_t charPositionInLine,
                        const std::string &msg, std::exception_ptr e) override;
        };

        class CovScriptCompiler : public BaseCompiler {
        private:
            CompilerErrorHandler _errorHandler;

        private:
            void constructASTs();

        public:
            CovScriptCompiler() = default;

            ~CovScriptCompiler() override;

            void addFile(const Ptr<SourceFile> &file) {
                _privateData.getCompileFiles()[file] = nullptr;
            }

            void compile();
        };
    }
}
