//
// Created by kiva on 2019-08-13.
//
#include <covscript/compiler/parser.h>
#include <covscript/compiler/sharedTypes.h>
#include <utility>
#include <sstream>

namespace cs {
    namespace compiler {
        class MyErrorStrategy : public antlr4::BailErrorStrategy {
        protected:
            void reportNoViableAlternative(antlr4::Parser *recognizer, const antlr4::NoViableAltException &e) override {
                std::string msg = "no available token";
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }

            void reportInputMismatch(antlr4::Parser *recognizer, const antlr4::InputMismatchException &e) override {
                std::string msg = "mismatched input " + getTokenErrorDisplay(e.getOffendingToken()) +
                                  " expecting " + e.getExpectedTokens().toString(recognizer->getVocabulary());
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }

            void reportFailedPredicate(antlr4::Parser *recognizer, const antlr4::FailedPredicateException &e) override {
                const std::string &ruleName = recognizer->getRuleNames()[recognizer->getContext()->getRuleIndex()];
                std::string msg = "rule " + ruleName + " " + e.what();
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }
        };

        Parser::Parser(SourceFile &file)
            : _sourceFile(file),
              _nativeStream(file.openNativeStream()),
              _antlrInputStream(*_nativeStream.get()),
              _lexer(&_antlrInputStream),
              _antlrTokenStream(&_lexer),
              CovScriptParser(&_antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
            _antlrInputStream.name = file.getSourceName();
        }

        Parser::~Parser() = default;

        void Parser::printSyntaxError(SyntaxError &e) {
            printf("Syntax error at file: %s:%zd:%zd\n",
                _sourceFile.getSourceName().c_str(),
                e.getLine(), e.getCharPosition());

            Ptr<std::istream> sourceStream = _sourceFile.openNativeStream();

//            if (is<const RegularSourceFile *>(&_sourceFile)) {
//                auto &file = static_cast<const RegularSourceFile &>(_sourceFile);
//                sourceStream = new std::ifstream(file.getFilePath());
//            } else if (is<const CodeSourceFile *>(&_sourceFile)) {
//                auto &file = static_cast<const CodeSourceFile &>(_sourceFile);
//                sourceStream = new std::istringstream(file.getCode());
//            } else if (is<const StreamSourceFile *>(&_sourceFile)) {
//
//            }

            int currentLine = 1;
            std::string line;
            while (std::getline(*sourceStream, line)) {
                if (currentLine == e.getLine()) {
                    break;
                }
                ++currentLine;
            }

            printf("    %s\n", line.c_str());
            printf("    ");
            for (int i = 0; i < e.getCharPosition(); ++i) {
                putchar(' ');
            }
            putchar('^');
            putchar('\n');
            printf("%s\n", e.getMessage().c_str());
        }

        SyntaxError::SyntaxError(antlr4::RuleContext *ruleContext, antlr4::Token *offendingToken,
                                 size_t line, size_t charPosition, std::string message)
            : _ruleContext(ruleContext), _offendingSymbol(offendingToken), _line(line),
              _charPosition(charPosition), _message(std::move(message)) {
        }

        CodeSourceFile::CodeSourceFile(std::string name, std::string code)
            : _name(std::move(name)), _code(std::move(code)) {
        }

        Ptr<std::istream> CodeSourceFile::openNativeStream() {
            return Ptr<std::istream>(new std::istringstream(_code));
        }

        RegularSourceFile::RegularSourceFile(std::string filePath)
            : _filePath(std::move(filePath)) {
        }

        Ptr<std::istream> RegularSourceFile::openNativeStream() {
            return Ptr<std::istream>(new std::ifstream(_filePath));
        }
    }
}
