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
            int32_t _hash = 0;
            Scope *_parent = nullptr;
            Scope *_nextSibling = nullptr;
            std::unordered_map<VMString, Ptr<Variable>> _variables;

        public:
            explicit Scope(Scope *parent);

            ~Scope() = default;

            Ptr<Variable> search(const VMString &name);

            Scope *getParent() { return _parent; };

            bool isRootScope() const { return getHash() == 0 && _parent == nullptr; }

            int32_t getHash() const { return _hash; }

            void setHash(int32_t hash) { _hash = hash; }

            Scope *newChild();

            Scope *findChild(int32_t hash);

            void preserve(const VMString &name);

            void preserve(const Parameter &param);

            void set(const VMString &param, const Argument &arg);

            void set(const Parameter &param, const Argument &arg);

            void set(const std::pair<Parameter, Argument> &pair);
        };
    }
}
