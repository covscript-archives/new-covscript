//
// Created by kiva on 2019/9/19.
//

#include <covscript/compiler/scope.h>

namespace cs {
    namespace compiler {
        Variable *Scope::search(const VMString &name) {
            auto iter = this->variables.find(name);
            return iter == this->variables.end() ? searchInParent(name) : iter->second;
        }

        Variable *Scope::searchInParent(const VMString &name) {
            return this->parent == nullptr ? nullptr : this->parent->search(name);
        }
    }
}
