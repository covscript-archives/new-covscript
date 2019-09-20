//
// Created by kiva on 2019/9/20.
//
#include <covscript/compiler/compiler.h>

namespace cs {
    namespace compiler {
        class CompilerErrorHandler : public antlr4::BaseErrorListener {
            void
            syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                        size_t charPositionInLine,
                        const std::string &msg, std::exception_ptr e) override {
                try {
                    std::rethrow_exception(e);
                } catch (antlr4::RecognitionException &re) {
                    // TODO: use CovSDK
                    throw cs::compiler::SyntaxError(re.getCtx(), offendingSymbol, line, charPositionInLine, msg);
                }
            }
        };

        CompilerPhase::CompilerPhase(VMString _phaseName)
            : _phaseName(std::move(_phaseName)) {
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
                    runPhase(phase, elem.second);
                    postPhase(phase);
                }
            }
        }

        void CovScriptCompiler::constructASTs() {
            CompilerErrorHandler errorHandler;

            for (auto &elem : _privateData.getCompileFiles()) {
                Parser parser(elem.first);

                parser.removeErrorListeners();
                parser.getLexer().removeErrorListeners();
                parser.addErrorListener(&errorHandler);
                parser.getLexer().addErrorListener(&errorHandler);

                try {
                    elem.second = parser.compilationUnit();

                } catch (SyntaxError &e) {
                    // TODO: use CovSDK
                    parser.printSyntaxError(e);
                }
            }
        }
    }
}
