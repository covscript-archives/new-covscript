//
// Created by kiva on 2019/9/27.
//
#pragma once

#include <covscript/compiler/utils/bytebuffer.h>

namespace cs {
    namespace compiler {
        class CodeWriter {
        private:
            ByteBuffer _byteBuffer;

        public:
            CodeWriter() = default;

            ~CodeWriter() = default;

            SizedBuffer getCodeBuffer() {
                return _byteBuffer.getBuffer();
            }
        };
    }
}
