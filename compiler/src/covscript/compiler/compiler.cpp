//
// Created by kiva on 2019/9/20.
//
#include <covscript/compiler/compiler.h>

namespace cs {
    namespace compiler {
        CompilerPhase::CompilerPhase(VMString _phaseName)
            : _phaseName(std::move(_phaseName)) {
        }

        void CompilerErrorHandler::syntaxError(antlr4::Recognizer *recognizer,
                                               antlr4::Token *offendingSymbol,
                                               size_t line, size_t charPositionInLine,
                                               const std::string &msg,
                                               std::exception_ptr e) {
            try {
                std::rethrow_exception(e);
            } catch (antlr4::RecognitionException &re) {
                // TODO: use CovSDK
                throw cs::compiler::SyntaxError(re.getCtx(), offendingSymbol, line, charPositionInLine, msg);
            }
        }

        void BaseCompiler::preparePhase(Ptr<CompilerPhase> &phase) {
            phase->preparePhase(_privateData);
        }

        void BaseCompiler::runPhase(Ptr<CompilerPhase> &phase,
                                    CovScriptParser::CompilationUnitContext *compilationUnit) {
            phase->runPhase(_privateData, compilationUnit);
        }

        void BaseCompiler::postPhase(Ptr<CompilerPhase> &phase) {
            phase->postPhase(_privateData);
        }

        void CovScriptCompiler::compile() {
            this->constructASTs();
            for (auto &phase : _compilerPhases) {
                for (auto &elem : _privateData.getCompileFiles()) {
                    preparePhase(phase);
                    runPhase(phase, elem.second->compilationUnit());
                    postPhase(phase);
                }
            }
        }

        void CovScriptCompiler::constructASTs() {
            for (auto &elem : _privateData.getCompileFiles()) {
                auto parser = new Parser(elem.first);

                parser->removeErrorListeners();
                parser->getLexer().removeErrorListeners();
                parser->addErrorListener(&_errorHandler);
                parser->getLexer().addErrorListener(&_errorHandler);

                try {
                    elem.second = parser;

                } catch (SyntaxError &e) {
                    // TODO: use CovSDK
                    parser->printSyntaxError(e);
                }
            }
        }

        CovScriptCompiler::~CovScriptCompiler() {
            for (auto &elem : _privateData.getCompileFiles()) {
                delete elem.second;
            }
        }
    }
}
