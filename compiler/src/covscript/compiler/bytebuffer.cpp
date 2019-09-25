//
// Created by kiva on 2019/8/25.
//
#include <covscript/compiler/bytebuffer.h>
#include <covscript/compiler/sharedTypes.h>
#include <cstring>
#include <cstdlib>
#include <sstream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

namespace cs {
    namespace compiler {
        ByteBuffer::ByteBuffer()
            : ByteBuffer(BUFFER_DEFAULT_LENGTH) {
        }

        ByteBuffer::ByteBuffer(size_t initialLength)
            : _buffer(nullptr), _bufferLength(initialLength), _position(0),
              _byteOrder(ByteOrder::BO_BIG_ENDIAN) {
            growBuffer();
        }

        ByteBuffer::~ByteBuffer() {
            if (_buffer != nullptr) {
                free(_buffer);
            }
        }

        void ByteBuffer::copy(const ByteBuffer::byte *from, ByteBuffer::byte *to, size_t count) {
            memcpy(to, from, count);
        }

        void ByteBuffer::growBuffer() {
            if (_buffer == nullptr) {
                // The buffer has not been allocated
                _buffer = static_cast<byte *>(malloc(sizeof(_buffer[0]) * _bufferLength));

                if (!_buffer) {
                    compilerThrow<RuntimeException>("growBuffer(): malloc failed");
                }
                return;
            }

            // The buffer needs to be expanded
            size_t currentLength = _bufferLength == 1 ? 2 : _bufferLength;
            size_t fitSize = currentLength * BUFFER_GROWTH_FACTOR;
            byte *fitBuffer = static_cast<byte *>(realloc(_buffer, sizeof(_buffer[0]) * fitSize));

            if (fitBuffer != nullptr) {
                // New buffer successfully allocated, the original buffer
                // was freed by realloc()
                _buffer = fitBuffer;
                _bufferLength = fitSize;
            } else {
                // Nothing changed when allocation failed
                compilerThrow<RuntimeException>("growBuffer(): realloc failed");
            }
        }

        void ByteBuffer::growIfNeeded(size_t least) {
            // Keep growing until current buffer can afford the required length.
            while (_position + least >= _bufferLength) {
                growBuffer();
            }
        }

        int8_t ByteBuffer::readInt8At(int index) {
            return _buffer[index];
        }

        int16_t ByteBuffer::readInt16At(int index) {
            return (_byteOrder == BO_BIG_ENDIAN ? readInt16AtBE(index) : readInt16AtLE(index));
        }

        int32_t ByteBuffer::readInt32At(int index) {
            return (_byteOrder == BO_BIG_ENDIAN ? readInt32AtBE(index) : readInt32AtLE(index));
        }

        int64_t ByteBuffer::readInt64At(int index) {
            return (_byteOrder == BO_BIG_ENDIAN ? readInt64AtBE(index) : readInt64AtLE(index));
        }

        float ByteBuffer::readFloatAt(int index) {
            return (_byteOrder == BO_BIG_ENDIAN ? readFloatAtBE(index) : readFloatAtLE(index));
        }

        double ByteBuffer::readDoubleAt(int index) {
            return (_byteOrder == BO_BIG_ENDIAN ? readDoubleAtBE(index) : readDoubleAtLE(index));
        }

        std::string ByteBuffer::readStringAt(int index, size_t length) {
            std::stringstream ss;
            for (size_t i = 0; i < length; ++i) {
                ss << _buffer[index++];
            }
            return ss.str();
        }

        void ByteBuffer::writeInt8At(int index, int8_t value) {
            growIfNeeded(1);
            _buffer[index] = value;
        }

        void ByteBuffer::writeInt16At(int index, int16_t value) {
            growIfNeeded(2);
            (_byteOrder == BO_BIG_ENDIAN) ? writeInt16AtBE(index, value) : writeInt16AtLE(index, value);
        }

        void ByteBuffer::writeInt32At(int index, int32_t value) {
            growIfNeeded(4);
            (_byteOrder == BO_BIG_ENDIAN) ? writeInt32AtBE(index, value) : writeInt32AtLE(index, value);
        }

        void ByteBuffer::writeInt64At(int index, int64_t value) {
            growIfNeeded(8);
            (_byteOrder == BO_BIG_ENDIAN) ? writeInt64AtBE(index, value) : writeInt64AtLE(index, value);
        }

        void ByteBuffer::writeFloatAt(int index, float value) {
            growIfNeeded(4);
            (_byteOrder == BO_BIG_ENDIAN) ? writeFloatAtBE(index, value) : writeFloatAtLE(index, value);
        }

        void ByteBuffer::writeDoubleAt(int index, double value) {
            growIfNeeded(8);
            (_byteOrder == BO_BIG_ENDIAN) ? writeDoubleAtBE(index, value) : writeDoubleAtLE(index, value);
        }

        void ByteBuffer::writeStringAt(int index, const std::string &value) {
            growIfNeeded(value.length());
            copy(reinterpret_cast<const byte *>(value.c_str()), _buffer + index, value.length());
        }

        ///////////////////////////////////////////////////////////////////////////////
        int16_t ByteBuffer::readInt16AtLE(int index) {
            byte b1 = _buffer[index + 1];
            byte b0 = _buffer[index];

            return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
        }

        int16_t ByteBuffer::readInt16AtBE(int index) {
            byte b0 = _buffer[index + 1];
            byte b1 = _buffer[index];

            return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
        }

        int32_t ByteBuffer::readInt32AtLE(int index) {
            byte b3 = _buffer[index + 3];
            byte b2 = _buffer[index + 2];
            byte b1 = _buffer[index + 1];
            byte b0 = _buffer[index];

            return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
        }

        int32_t ByteBuffer::readInt32AtBE(int index) {
            byte b0 = _buffer[index + 3];
            byte b1 = _buffer[index + 2];
            byte b2 = _buffer[index + 1];
            byte b3 = _buffer[index];

            return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
        }

        int64_t ByteBuffer::readInt64AtLE(int index) {
            byte b7 = _buffer[index + 7];
            byte b6 = _buffer[index + 6];
            byte b5 = _buffer[index + 5];
            byte b4 = _buffer[index + 4];
            byte b3 = _buffer[index + 3];
            byte b2 = _buffer[index + 2];
            byte b1 = _buffer[index + 1];
            byte b0 = _buffer[index];

            return (((int64_t) b7 & 0xFF) << 56) | (((int64_t) b6 & 0xFF) << 48) |
                   (((int64_t) b5 & 0xFF) << 40) | (((int64_t) b4 & 0xFF) << 32) |
                   (((int64_t) b3 & 0xFF) << 24) | (((int64_t) b2 & 0xFF) << 16) |
                   (((int64_t) b1 & 0xFF) << 8) | ((int64_t) b0 & 0xFF);
        }

        int64_t ByteBuffer::readInt64AtBE(int index) {
            byte b0 = _buffer[index + 7];
            byte b1 = _buffer[index + 6];
            byte b2 = _buffer[index + 5];
            byte b3 = _buffer[index + 4];
            byte b4 = _buffer[index + 3];
            byte b5 = _buffer[index + 2];
            byte b6 = _buffer[index + 1];
            byte b7 = _buffer[index];

            return (((int64_t) b7 & 0xFF) << 56) | (((int64_t) b6 & 0xFF) << 48) |
                   (((int64_t) b5 & 0xFF) << 40) | (((int64_t) b4 & 0xFF) << 32) |
                   (((int64_t) b3 & 0xFF) << 24) | (((int64_t) b2 & 0xFF) << 16) |
                   (((int64_t) b1 & 0xFF) << 8) | ((int64_t) b0 & 0xFF);
        }

        float ByteBuffer::readFloatAtLE(int index) {
            union {
                byte bytes[4];
                float value;
            } bits{};

            for (int i = 0; i < 4; i++) {
                bits.bytes[i] = _buffer[index++];
            }

            return bits.value;
        }

        float ByteBuffer::readFloatAtBE(int index) {
            union {
                byte bytes[4];
                float value;
            } bits{};

            for (int i = 3; i >= 0; i--) {
                bits.bytes[i] = _buffer[index++];
            }

            return bits.value;
        }

        double ByteBuffer::readDoubleAtLE(int index) {
            union {
                byte bytes[8];
                double value;
            } bits{};

            for (int i = 0; i < 8; i++) {
                bits.bytes[i] = _buffer[index++];
            }

            return bits.value;
        }

        double ByteBuffer::readDoubleAtBE(int index) {
            union {
                byte bytes[8];
                double value;
            } bits{};

            for (int i = 7; i >= 0; i--) {
                bits.bytes[i] = _buffer[index++];
            }

            return bits.value;
        }

        ///////////////////////////////////////////////////////////////////////////////

        void ByteBuffer::writeInt16AtLE(int index, int16_t value) {
            _buffer[index++] = value & 0xFF;
            _buffer[index] = (value >> 8) & 0xFF;
        }

        void ByteBuffer::writeInt16AtBE(int index, int16_t value) {
            _buffer[index++] = (value >> 8) & 0xFF;
            _buffer[index] = value & 0xFF;
        }

        void ByteBuffer::writeInt32AtLE(int index, int32_t value) {
            _buffer[index++] = value & 0xFF;
            _buffer[index++] = (value >> 8) & 0xFF;
            _buffer[index++] = (value >> 16) & 0xFF;
            _buffer[index] = (value >> 24) & 0xFF;
        }

        void ByteBuffer::writeInt32AtBE(int index, int32_t value) {
            _buffer[index++] = (value >> 24) & 0xFF;
            _buffer[index++] = (value >> 16) & 0xFF;
            _buffer[index++] = (value >> 8) & 0xFF;
            _buffer[index] = value & 0xFF;
        }

        void ByteBuffer::writeInt64AtLE(int index, int64_t value) {
            _buffer[index++] = value & 0xFF;
            _buffer[index++] = (value >> 8) & 0xFF;
            _buffer[index++] = (value >> 16) & 0xFF;
            _buffer[index++] = (value >> 24) & 0xFF;
            _buffer[index++] = (value >> 32) & 0xFF;
            _buffer[index++] = (value >> 40) & 0xFF;
            _buffer[index++] = (value >> 48) & 0xFF;
            _buffer[index] = (value >> 56) & 0xFF;
        }

        void ByteBuffer::writeInt64AtBE(int index, int64_t value) {
            _buffer[index++] = (value >> 56) & 0xFF;
            _buffer[index++] = (value >> 48) & 0xFF;
            _buffer[index++] = (value >> 40) & 0xFF;
            _buffer[index++] = (value >> 32) & 0xFF;
            _buffer[index++] = (value >> 24) & 0xFF;
            _buffer[index++] = (value >> 16) & 0xFF;
            _buffer[index++] = (value >> 8) & 0xFF;
            _buffer[index] = value & 0xFF;
        }

        void ByteBuffer::writeFloatAtLE(int index, float value) {
            union {
                byte bytes[4];
                float value;
            } bits{};

            bits.value = value;
            for (int i = 0; i < 4; i++) {
                _buffer[index++] = bits.bytes[i];
            }
        }

        void ByteBuffer::writeFloatAtBE(int index, float value) {
            union {
                byte bytes[4];
                float value;
            } bits{};

            bits.value = value;
            for (int i = 3; i >= 0; i--) {
                _buffer[index++] = bits.bytes[i];
            }
        }

        void ByteBuffer::writeDoubleAtLE(int index, double value) {
            union {
                byte bytes[8];
                double value;
            } bits{};

            bits.value = value;
            for (int i = 0; i < 8; i++) {
                _buffer[index++] = bits.bytes[i];
            }
        }

        void ByteBuffer::writeDoubleAtBE(int index, double value) {
            union {
                byte bytes[8];
                double value;
            } bits{};

            bits.value = value;
            for (int i = 7; i >= 0; i--) {
                _buffer[index++] = bits.bytes[i];
            }
        }
    }
}

#pragma clang diagnostic pop
