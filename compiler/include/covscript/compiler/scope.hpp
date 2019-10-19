//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/variable.hpp>
#include <covscript/compiler/parser.hpp>
#include <unordered_map>
#include <utility>

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
            CovScriptParser::ExpressionContext *_expr = nullptr;

        public:
            CovScriptParser::ExpressionContext *getExpr() const {
                return _expr;
            }
        };

        class Scope {
        private:
            Scope *_parent;
            std::unordered_map<VMString, Ptr<Variable>> _variables;

        public:
            explicit Scope(Scope *parent);

            ~Scope() = default;

            Ptr<Variable> search(const VMString &name);

            Scope *getParent() { return _parent; };

            Scope *newChild();

            void preserve(const VMString &name);

            void preserve(const Parameter &param);

            void set(const VMString &param, const Argument &arg);

            void set(const Parameter &param, const Argument &arg);

            void set(const std::pair<Parameter, Argument> &pair);
        };
    }
}
