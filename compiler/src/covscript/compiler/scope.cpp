//
// Created by kiva on 2019/9/19.
//

#include <covscript/compiler/scope.hpp>

namespace cs {
    namespace compiler {
        Scope::Scope(Scope *parent)
            : _parent(parent) {
        }

        Ptr<Variable> Scope::search(const VMString &name) {
            auto iter = _variables.find(name);
            return iter == _variables.end()
                   ? (_parent == nullptr ? nullptr : _parent->search(name))
                   : iter->second;
        }

        void Scope::set(const Parameter &param, const Argument &arg) {
            set(param.getName(), arg);
        }

        void Scope::set(const std::pair<Parameter, Argument> &pair) {
            set(pair.first, pair.second);
        }

        void Scope::set(const VMString &param, const Argument &arg) {
            this->_variables.insert(std::make_pair(param, makePtr<Variable>(param, arg.getExpr())));
        }

        void Scope::preserve(const VMString &name) {
            set(name, Argument());
        }

        void Scope::preserve(const Parameter &param) {
            preserve(param.getName());
        }

        Scope *Scope::newChild() {
            return new Scope(this);
        }
    }
}
