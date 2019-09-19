//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/compiler/variable.h>
#include <unordered_map>

namespace cs {
    namespace compiler {
        class Scope {
        private:
            Scope *parent;
            std::unordered_map<VMString, Variable *> variables;

        private:
            Variable* searchInParent(const VMString &name);

        public:
            Variable* search(const VMString &name);
        };
    }
}
