//
// Created by kiva on 2019/11/8.
//

#include <covscript/compiler/compiler.hpp>
#include <sstream>
#include <string>

namespace cs {
    namespace compiler {
        std::string CovScriptCompiler::formatSyntaxError(const SyntaxError &error) {
            std::stringstream ss;
            ss << "Syntax error at file: " << error.getErrorFile()
                << ":" << error.getLine()
                << ":" << error.getCharPosition()
                << std::endl;

            ss <<  "    " << error.getErrorCode() << std::endl;
            ss <<  "    ";
            for (size_t i = 0; i < error.getCharPosition(); ++i) {
                ss << " ";
            }
            ss <<  "^" << std::endl << error.getMessage() << std::endl;
            return ss.str();
        }
    }
}
