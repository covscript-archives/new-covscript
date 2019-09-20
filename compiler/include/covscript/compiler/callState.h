//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/compiler/types.h>
#include <covscript/compiler/parser.h>

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
