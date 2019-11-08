//
// Created by kiva on 2019-08-13.
//
#include <covscript/compiler/parser.hpp>
#include <covscript/compiler/sharedTypes.hpp>
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
                std::string msg = "unexpected input " + getTokenErrorDisplay(e.getOffendingToken()) +
                                  " expected " + e.getExpectedTokens().toString(recognizer->getVocabulary());
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }

            void reportFailedPredicate(antlr4::Parser *recognizer, const antlr4::FailedPredicateException &e) override {
                const std::string &ruleName = recognizer->getRuleNames()[recognizer->getContext()->getRuleIndex()];
                std::string msg = "rule " + ruleName + " " + e.what();
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }
        };

        Parser::Parser(const Ptr<SourceFile>& file)
            : _sourceFile(file),
              _nativeStream(file->openNativeStream()),
              _antlrInputStream(*_nativeStream.get()),
              _lexer(&_antlrInputStream),
              _antlrTokenStream(&_lexer),
              CovScriptParser(&_antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
            _antlrInputStream.name = file->getSourceName();
        }

        Parser::~Parser() = default;

        void Parser::printSyntaxError(SyntaxError &e) {
            printf("Syntax error at file: %s:%zd:%zd\n",
                _sourceFile->getSourceName().c_str(),
                e.getLine(), e.getCharPosition());

            Ptr<std::istream> sourceStream = _sourceFile->openNativeStream();

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
            for (size_t i = 0; i < e.getCharPosition(); ++i) {
                putchar(' ');
            }
            printf("^\n%s\n", e.getMessage().c_str());
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

        StreamSourceFile::StreamSourceFile(std::string streamName, Ptr<std::istream> stream)
            : _streamName(std::move(streamName)), _stream(std::move(stream)) {
        }

        Ptr<std::istream> RegularSourceFile::openNativeStream() {
            return Ptr<std::istream>(new std::ifstream(_filePath));
        }
    }
}
