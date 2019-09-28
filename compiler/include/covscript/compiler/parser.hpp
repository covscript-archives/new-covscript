//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <antlr4-runtime.h>
#include <covscript/compiler/sharedTypes.hpp>
#include <CovScriptParser.h>
#include <CovScriptLexer.h>
#include <CovScriptBaseListener.h>
#include <CovScriptBaseVisitor.h>

namespace cs {
    namespace compiler {
        using namespace cs_compiler_antlr_gen;

        class SourceFile {
        public:
            virtual Ptr<std::istream> openNativeStream() = 0;

            virtual const std::string &getSourceName() = 0;
        };

        class CodeSourceFile : public SourceFile {
        private:
            std::string _name;
            std::string _code;

        public:
            CodeSourceFile(std::string name, std::string code);

            const std::string &getCode() const {
                return _code;
            }

            Ptr<std::istream> openNativeStream() override;

            const std::string &getSourceName() override {
                return _name;
            }
        };

        class RegularSourceFile : public SourceFile {
        private:
            std::string _filePath;

        public:
            explicit RegularSourceFile(std::string filePath);

            const std::string &getFilePath() const {
                return _filePath;
            }

            Ptr<std::istream> openNativeStream() override;

            const std::string &getSourceName() override {
                return _filePath;
            }
        };

        class StreamSourceFile : public SourceFile {
        private:
            std::string _streamName;
            Ptr<std::istream> _stream;

        public:
            StreamSourceFile(std::string streamName, Ptr<std::istream> stream);

            Ptr<std::istream> openNativeStream() override {
                return _stream;
            }

            const std::string &getSourceName() override {
                return _streamName;
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
            Ptr<SourceFile> _sourceFile;
            CovScriptLexer _lexer;
            Ref<std::istream> _nativeStream;
            antlr4::ANTLRInputStream _antlrInputStream;
            antlr4::CommonTokenStream _antlrTokenStream;

        public:
            explicit Parser(const Ptr<SourceFile>& file);

            ~Parser() override;

            void printSyntaxError(SyntaxError &e);

            CovScriptLexer &getLexer() {
                return _lexer;
            }

            Ptr<SourceFile> getSourceFile() {
                return _sourceFile;
            }
        };
    }
}
