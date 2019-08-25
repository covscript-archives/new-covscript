//
// Created by kiva on 2019/8/25.
//

#include <covscript/compiler/bytebuffer.h>
#include "BenchmarkHelper.h"

using namespace cs::compiler;

void bench() {
    ByteBuffer buffer(2);
    buffer.setOrder(ByteOrder::BO_BIG_ENDIAN);
    buffer.writeInt8('h');
    buffer.writeInt8('e');
    buffer.writeInt8('l');
    buffer.writeInt8('l');
    buffer.writeInt8('o');
    buffer.writeInt16(10086);
    buffer.writeInt32(10010);
    buffer.writeInt64(10000);
    size_t position = buffer.getPosition();
    buffer.writeInt32(0);
    buffer.writeFloat(3.14f);
    buffer.writeDouble(6.28);
    buffer.writeInt32At(position, 520);
    buffer.rewind();

    assert(buffer.readInt8() == 'h');
    assert(buffer.readInt8() == 'e');
    assert(buffer.readInt8() == 'l');
    assert(buffer.readInt8() == 'l');
    assert(buffer.readInt8() == 'o');
    assert(buffer.readInt16() == 10086);
    assert(buffer.readInt32() == 10010);
    assert(buffer.readInt64() == 10000);
    assert(buffer.readInt32() == 520);
    assert(buffer.readFloat() == 3.14f);
    assert(buffer.readDouble() == 6.28);
}

int main() {
    cs::benchmark("ByteBuffer", bench);
}
