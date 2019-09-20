//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <antlr4-runtime.h>
#include <exception>
#include <stdexcept>
#include <CovScriptParser.h>
#include <CovScriptLexer.h>

namespace cs {
    namespace compiler {
        using namespace cs_compiler_antlr_gen;

        class SourceFile {
        private:
            bool _isFileSource;
            std::string _source;

        public:
            explicit SourceFile(std::string sourceName, bool isFileSource = true);

            const std::string &getSource() const {
                return _source;
            }

            bool isFile() const {
                return _isFileSource;
            }
        };

        class SyntaxError : public std::exception {
        private:
            antlr4::RuleContext *_ruleContext;
            antlr4::Token *_offendingSymbol;
            size_t _line;
            size_t _charPosition;
            std::string _message;

        public:
            SyntaxError(antlr4::RuleContext *ruleContext, antlr4::Token *offendingSymbol,
                        size_t line, size_t charPosition, std::string message);

            antlr4::RuleContext *getRuleContext() const {
                return _ruleContext;
            }

            size_t getLine() const {
                return _line;
            }

            size_t getCharPosition() const {
                return _charPosition;
            }

            const std::string &getMessage() const {
                return _message;
            }

            antlr4::Token *getOffendingSymbol() const {
                return _offendingSymbol;
            }
        };

        class Parser : public CovScriptParser {
        private:
            std::istream *_stream;
            SourceFile _sourceFile;
            CovScriptLexer _lexer;
            antlr4::ANTLRInputStream _antlrInputStream;
            antlr4::CommonTokenStream _antlrTokenStream;

        public:
            explicit Parser(SourceFile file);

            explicit Parser(const std::string &code);

            virtual ~Parser() override;

            void printSyntaxError(SyntaxError &e);

            CovScriptLexer &getLexer() {
                return _lexer;
            }

            SourceFile &getSourceFile() {
                return _sourceFile;
            }
        };
    }
}
