//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <antlr4-runtime.h>
#include <covscript/compiler/file.h>
#include <exception>
#include <stdexcept>
#include <CovScriptParser.h>
#include <CovScriptLexer.h>

namespace cs {
    namespace compiler {
        using namespace cs_compiler_antlr_gen;

        class SyntaxError : public std::exception {
        private:
            antlr4::RuleContext *ruleContext;
            antlr4::Token *offendingSymbol;
            size_t line;
            size_t charPosition;
            std::string message;

        public:
            SyntaxError(antlr4::RuleContext *ruleContext, antlr4::Token *offendingSymbol,
                        size_t line, size_t charPosition, std::string message);

            antlr4::RuleContext *getRuleContext() const {
                return ruleContext;
            }

            size_t getLine() const {
                return line;
            }

            size_t getCharPosition() const {
                return charPosition;
            }

            const std::string &getMessage() const {
                return message;
            }

            antlr4::Token *getOffendingSymbol() const {
                return offendingSymbol;
            }
        };

        class Parser : public CovScriptParser {
        private:
            std::istream *stream;
            SourceFile sourceFile;
            CovScriptLexer lexer;
            antlr4::ANTLRInputStream antlrInputStream;
            antlr4::CommonTokenStream antlrTokenStream;

        public:
            explicit Parser(SourceFile file);

            explicit Parser(const std::string &code);

            virtual ~Parser() override;

            void printSyntaxError(SyntaxError &e);

            CovScriptLexer &getLexer() {
                return lexer;
            }

            SourceFile &getSourceFile() {
                return sourceFile;
            }
        };
    }
}
