//
// Created by kiva on 2019/10/24.
//

#pragma once

#include <covscript/compiler/parser.hpp>
#include <unordered_map>

namespace cs {
    namespace compiler {
        class CFGElement;
        class CFGBasicBlock;
        class CFGLoop;
        class CFGEdge;

        class CFGBlockPool {
        private:
        };

        class CFGElement {
        public:
            virtual bool isBlock() { return false; }

            virtual bool isLoop() { return false; }

            CFGBasicBlock *asBlock() {
                if (!isBlock()) {
                    compilerThrow<cs::runtime_error>("must be a block");
                }
                return reinterpret_cast<CFGBasicBlock *>(this);
            }

            CFGLoop *asLoop() {
                if (!isLoop()) {
                    compilerThrow<cs::runtime_error>("must be a loop");
                }
                return reinterpret_cast<CFGLoop *>(this);
            }

            virtual ~CFGElement() = default;
        };

        class CFGBasicBlock : public CFGElement {
        private:
        };

        class CFGLoop : public CFGElement {
        private:
        };

        class CFGEdge : public CFGElement {
        private:
        };
    }
}
