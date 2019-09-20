//
// Created by kiva on 2019-08-13.
//
#include <covscript/compiler/parser.h>
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
                const std::string& ruleName = recognizer->getRuleNames()[recognizer->getContext()->getRuleIndex()];
                std::string msg = "rule " + ruleName + " " + e.what();
                recognizer->notifyErrorListeners(e.getOffendingToken(), msg, std::make_exception_ptr(e));
            }
        };

        Parser::Parser(SourceFile file)
            : _sourceFile(std::move(file)),
              _stream(new std::ifstream(file.getSource())),
              _antlrInputStream(*_stream),
              _lexer(&_antlrInputStream),
              _antlrTokenStream(&_lexer),
              CovScriptParser(&_antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
        }

        Parser::Parser(const std::string &code)
            : _sourceFile(code, false),
              _stream(nullptr),
              _antlrInputStream(code),
              _lexer(&_antlrInputStream),
              _antlrTokenStream(&_lexer),
              CovScriptParser(&_antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
        }

        Parser::~Parser() {
            delete _stream;
        }

        void Parser::printSyntaxError(SyntaxError &e) {
            const auto &source = _sourceFile.isFile() ? _sourceFile.getSource() : "<unknown>";

            printf("Syntax error at file: %s:%zd:%zd\n",
                source.c_str(), e.getLine(), e.getCharPosition());

            std::istream *sourceStream = nullptr;
            if (getSourceFile().isFile()) {
                sourceStream = new std::ifstream(source);
            } else {
                sourceStream = new std::istringstream(source);
            }

            int currentLine = 1;
            std::string line;
            while (std::getline(*sourceStream, line)) {
                if (currentLine == e.getLine()) {
                    break;
                }
                ++currentLine;
            }

            delete sourceStream;

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

        SourceFile::SourceFile(std::string sourceName, bool isFileSource)
            : _source(std::move(sourceName)), _isFileSource(isFileSource) {
        }
    }
}
