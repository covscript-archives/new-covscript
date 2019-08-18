//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <antlr4-runtime.h>
#include <covscript/compiler/file.h>
#include "CovScriptParser.h"
#include "CovScriptLexer.h"

namespace cs {
    namespace compiler {
        class Parser : public CovScriptParser {
        private:
            std::istream *stream;
            SourceFile sourceFile;
            CovScriptLexer lexer;
            antlr4::ANTLRInputStream antlrInputStream;
            antlr4::CommonTokenStream antlrTokenStream;

        public:
            explicit Parser(SourceFile file);

            explicit Parser(const std::string& code);

            virtual ~Parser() override;
        };
    }
}
