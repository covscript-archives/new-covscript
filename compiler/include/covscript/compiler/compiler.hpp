//
// Created by kiva on 2019/9/20.
//
#pragma once

#ifndef NDEBUG
#define COVSCRIPT_SDK_DEBUG
#endif

#ifdef CS_COMPILER_PHASE_IMPL
#define PHASE_LOG(msg) COVSDK_LOGEV(msg)
#else
#define PHASE_LOG(msg)
#endif

#include <covscript/compiler/scope.hpp>
#include <covscript/compiler/parser.hpp>
#include <covscript/shared/utility.hpp>
#include <unordered_map>
#include <list>

namespace cs {
    namespace compiler {
        enum class OptimizationLevel {
            NONE_OPT,
            SPEED_FIRST,
            QUALITY_FIRST,
        };

        /**
         * The data holder shared between all phases.
         */
        class CompilerData {
        public:
            using CompileFiles = std::unordered_map<Ptr<SourceFile>,
                std::pair<Parser *, Parser::CompilationUnitContext *>>;

        private:
            bool _verboseOutput = false;
            OptimizationLevel _optLevel;
            Scope *_globalScope = nullptr;
            CompileFiles _compileFiles;

        public:
            Scope *getGlobalScope() { return _globalScope; }

            CompileFiles &getCompileFiles() { return _compileFiles; }

            bool isVerboseOutput() const {
                return _verboseOutput;
            }

            void setVerboseOutput(bool verboseOutput) {
                _verboseOutput = verboseOutput;
            }

            OptimizationLevel getOptimizationLevel() const {
                return _optLevel;
            }

            void setOptimizationLevel(OptimizationLevel level) {
                _optLevel = level;
            }

            bool isOptimizationEnabled() {
                return getOptimizationLevel() != OptimizationLevel::NONE_OPT;
            }

            bool isSpeedOptimization() {
                return getOptimizationLevel() == OptimizationLevel::SPEED_FIRST;
            }

            bool isQualityOptimization() {
                return getOptimizationLevel() == OptimizationLevel::QUALITY_FIRST;
            }
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

            virtual void preparePhase(CompilerData &compilerData) = 0;

            virtual void runPhase(CompilerData &compilerData,
                                  CovScriptParser::CompilationUnitContext *compilationUnit) = 0;

            virtual void postPhase(CompilerData &compilerData) = 0;

        public:
            const VMString &getPhaseName() {
                return _phaseName;
            }
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

            void addPhase(const Ptr<CompilerPhase> &phase) {
                _compilerPhases.push_back(phase);
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
                _privateData.getCompileFiles()[file] = std::make_pair(nullptr, nullptr);
            }

            void setVerbose(bool verbose) {
                _privateData.setVerboseOutput(verbose);
            }

            void setOptimizationLevel(OptimizationLevel level) {
                _privateData.setOptimizationLevel(level);
            }

            void compile();
        };
    }
}
