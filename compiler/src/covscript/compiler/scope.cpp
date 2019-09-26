//
// Created by kiva on 2019/9/19.
//

#include <covscript/compiler/scope.h>

namespace cs {
    namespace compiler {
        Variable *Scope::search(const VMString &name) {
            auto iter = _variables.find(name);
            return iter == _variables.end()
                   ? (_parent == nullptr ? nullptr : _parent->search(name))
                   : iter->second;
        }

        void Scope::set(const Parameter &param, const Argument &arg) {
            // TODO: please implement variable framework
        }

        void Scope::set(const std::pair<Parameter, Argument> &pair) {
            set(pair.first, pair.second);
        }
    }
}
