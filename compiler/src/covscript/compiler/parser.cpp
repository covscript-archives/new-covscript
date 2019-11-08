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
        private:
            // DO NOT use this field in destructor
            Parser *_parser = nullptr;

        public:
            explicit MyErrorStrategy(Parser *parser) : _parser(parser) {}

        protected:
            void reportNoViableAlternative(antlr4::Parser *recognizer, const antlr4::NoViableAltException &e) override {
                std::string msg = "unexpected token";

                ParserErrorData errorData(e,
                    _parser->getTokenStream()->getText(e.getStartToken(), e.getOffendingToken()),
                    _parser->getSourceName());

                recognizer->notifyErrorListeners(e.getOffendingToken(), msg,
                    std::make_exception_ptr(errorData));
            }

            void reportInputMismatch(antlr4::Parser *recognizer, const antlr4::InputMismatchException &e) override {
                std::string msg = "unexpected input " + getTokenErrorDisplay(e.getOffendingToken()) +
                                  " expected " + e.getExpectedTokens().toString(recognizer->getVocabulary());

                ParserErrorData errorData(e,
                    _parser->getTokenStream()->getText(e.getCtx()),
                    _parser->getSourceName());

                recognizer->notifyErrorListeners(e.getOffendingToken(), msg,
                    std::make_exception_ptr(errorData));
            }

            void reportFailedPredicate(antlr4::Parser *recognizer, const antlr4::FailedPredicateException &e) override {
                const std::string &ruleName = recognizer->getRuleNames()[recognizer->getContext()->getRuleIndex()];
                std::string msg = "rule " + ruleName + " " + e.what();

                ParserErrorData errorData(e,
                    _parser->getTokenStream()->getText(e.getCtx()),
                    _parser->getSourceName());

                recognizer->notifyErrorListeners(e.getOffendingToken(), msg,
                    std::make_exception_ptr(errorData));
            }
        };

        Parser::Parser(const Ptr<SourceFile> &file)
            : _sourceFile(file),
              _nativeStream(file->openNativeStream()),
              _antlrInputStream(*_nativeStream.get()),
              _lexer(&_antlrInputStream),
              _antlrTokenStream(&_lexer),
              CovScriptParser(&_antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>(this));
            _antlrInputStream.name = file->getSourceName();
        }

        Parser::~Parser() = default;

        SyntaxError::SyntaxError(antlr4::RuleContext *ruleContext, antlr4::Token *offendingToken,
                                 size_t line, size_t charPosition, std::string message,
                                 std::string errorCode, std::string errorFile)
            : _ruleContext(ruleContext), _offendingSymbol(offendingToken), _line(line),
              _charPosition(charPosition), _message(std::move(message)),
              _errorCode(std::move(errorCode)), _errorFile(std::move(errorFile)) {
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
