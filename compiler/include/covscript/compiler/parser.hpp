//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <antlr4-runtime.h>
#include <covscript/compiler/sharedTypes.hpp>
#include <utility>
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

        class ParserErrorData {
        public:
            const antlr4::RecognitionException &_reEx;
            std::string _errorCode;
            std::string _errorFile;

            ParserErrorData(const antlr4::RecognitionException &reEx, std::string errorCode,
                            std::string errorFile)
                : _reEx(reEx), _errorCode(std::move(errorCode)), _errorFile(std::move(errorFile)) {
            }
        };

        class SyntaxError : public std::exception {
        private:
            antlr4::RuleContext *_ruleContext;
            antlr4::Token *_offendingSymbol;
            size_t _line;
            size_t _charPosition;
            std::string _message;
            std::string _errorCode;
            std::string _errorFile;

        public:
            SyntaxError(antlr4::RuleContext *ruleContext, antlr4::Token *offendingSymbol,
                        size_t line, size_t charPosition, std::string message,
                        std::string errorCode, std::string errorFile);

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

            const std::string &getErrorCode() const {
                return _errorCode;
            }

            const std::string &getErrorFile() const {
                return _errorFile;
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
            explicit Parser(const Ptr<SourceFile> &file);

            ~Parser() override;

            CovScriptLexer &getLexer() {
                return _lexer;
            }

            Ptr<SourceFile> getSourceFile() {
                return _sourceFile;
            }
        };
    }
}
