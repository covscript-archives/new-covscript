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
            : sourceFile(std::move(file)),
              stream(new std::ifstream(file.getSource())),
              antlrInputStream(*stream),
              lexer(&antlrInputStream),
              antlrTokenStream(&lexer),
              CovScriptParser(&antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
        }

        Parser::Parser(const std::string &code)
            : sourceFile(code, false),
              stream(nullptr),
              antlrInputStream(code),
              lexer(&antlrInputStream),
              antlrTokenStream(&lexer),
              CovScriptParser(&antlrTokenStream) {
            setErrorHandler(std::make_shared<MyErrorStrategy>());
        }

        Parser::~Parser() {
            delete stream;
        }

        void Parser::printSyntaxError(SyntaxError &e) {
            const auto &source = sourceFile.isFile() ? sourceFile.getSource() : "<unknown>";

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
            : ruleContext(ruleContext), offendingSymbol(offendingToken), line(line),
              charPosition(charPosition), message(std::move(message)) {
        }
    }
}
