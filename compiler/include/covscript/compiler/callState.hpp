//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/compiler/sharedTypes.hpp>
#include <covscript/compiler/parser.hpp>

namespace cs {
    namespace compiler {
        class Parameter {
        private:
            VMString _paramName;

        public:
            const VMString &getName() const { return _paramName; }
        };

        class Argument {
        private:
            CovScriptParser::ExpressionContext *_expr;
        };
    }
}
