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
                   ? (isRootScope() ? nullptr : _parent->search(name))
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
            auto child = new Scope(this);
            // Add child to the head
            child->_nextSibling = _nextSibling;
            _nextSibling = child;
            return child;
        }

        Scope *Scope::findChild(int32_t hash) {
            Scope *current = _nextSibling;
            while (current) {
                if (current->getHash() == hash) {
                    return current;
                }
                current = current->_nextSibling;
            }
            return nullptr;
        }
    }
}
