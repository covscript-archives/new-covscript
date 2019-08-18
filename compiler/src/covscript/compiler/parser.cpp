//
// Created by kiva on 2019-08-13.
//
#include <covscript/compiler/parser.h>
#include <utility>

namespace cs {
    namespace compiler {
        Parser::Parser(SourceFile file)
            : sourceFile(std::move(file)),
              stream(new std::ifstream(file.getSourceName())),
              antlrInputStream(*stream),
              lexer(&antlrInputStream),
              antlrTokenStream(&lexer),
              CovScriptParser(&antlrTokenStream) {
        }

        Parser::Parser(const std::string &code)
            : sourceFile("<code>"),
              stream(nullptr),
              antlrInputStream(code),
              lexer(&antlrInputStream),
              antlrTokenStream(&lexer),
              CovScriptParser(&antlrTokenStream) {
        }

        Parser::~Parser() {
            delete stream;
        }
    }
}
