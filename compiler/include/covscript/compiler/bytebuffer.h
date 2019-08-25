//
// Created by kiva on 2019/8/25.
//

#pragma once

#include <cstdio>
#include <cstdint>
#include <string>

namespace cs {
    namespace compiler {
        enum ByteOrder {
            BO_LITTLE_ENDIAN, BO_BIG_ENDIAN
        };

        class ByteBuffer {
        public:
            using byte = uint8_t;

            static constexpr float BUFFER_GROWTH_FACTOR = 1.5f;
            static constexpr size_t BUFFER_DEFAULT_LENGTH = 128;

        private:
            byte *buffer;
            size_t bufferLength;
            size_t position;
            ByteOrder byteOrder;

        private:
            void growBuffer();

            void growIfNeeded(size_t least);

            static void copy(const byte *from, byte *to, size_t count);

        public:
            ByteBuffer();

            explicit ByteBuffer(size_t initialLength);

            ~ByteBuffer();

            void setOrder(ByteOrder order) {
                this->byteOrder = order;
            }

            ByteOrder getByteOrder() const {
                return byteOrder;
            }

            size_t getLength() const {
                return bufferLength;
            }

            size_t getPosition() const {
                return position;
            }

            void rewind() {
                position = 0;
            }

        private:
            int16_t readInt16AtLE(int index);

            int16_t readInt16AtBE(int index);

            int32_t readInt32AtLE(int index);

            int32_t readInt32AtBE(int index);

            int64_t readInt64AtLE(int index);

            int64_t readInt64AtBE(int index);

            float readFloatAtLE(int index);

            float readFloatAtBE(int index);

            double readDoubleAtLE(int index);

            double readDoubleAtBE(int index);

            void writeInt16AtLE(int index, int16_t value);

            void writeInt16AtBE(int index, int16_t value);

            void writeInt32AtLE(int index, int32_t value);

            void writeInt32AtBE(int index, int32_t value);

            void writeInt64AtLE(int index, int64_t value);

            void writeInt64AtBE(int index, int64_t value);

            void writeFloatAtLE(int index, float value);

            void writeFloatAtBE(int index, float value);

            void writeDoubleAtLE(int index, double value);

            void writeDoubleAtBE(int index, double value);

        public:
            int8_t readInt8() {
                return readInt8At(position++);
            }

            int16_t readInt16() {
                auto r = readInt16At(position);
                position += 2;
                return r;
            }

            int32_t readInt32() {
                auto r = readInt32At(position);
                position += 4;
                return r;
            }

            int64_t readInt64() {
                auto r = readInt64At(position);
                position += 8;
                return r;
            }

            float readFloat() {
                auto r = readFloatAt(position);
                position += 4;
                return r;
            }

            double readDouble() {
                auto r = readDoubleAt(position);
                position += 8;
                return r;
            }

            std::string readString(size_t length) {
                const auto &r = readStringAt(position, length);
                position += length;
                return r;
            }

            int8_t readInt8At(int index);

            int16_t readInt16At(int index);

            int32_t readInt32At(int index);

            int64_t readInt64At(int index);

            float readFloatAt(int index);

            double readDoubleAt(int index);

            std::string readStringAt(int index, size_t length);

        public:
            void writeInt8(int8_t value) {
                writeInt8At(position++, value);
            }

            void writeInt16(int16_t value) {
                writeInt16At(position, value);
                position += 2;
            }

            void writeInt32(int32_t value) {
                writeInt32At(position, value);
                position += 4;
            }

            void writeInt64(int64_t value) {
                writeInt64At(position, value);
                position += 8;
            }

            void writeFloat(float value) {
                writeFloatAt(position, value);
                position += 4;
            }

            void writeDouble(double value) {
                writeDoubleAt(position, value);
                position += 8;
            }

            void writeString(const std::string &value) {
                writeStringAt(position, value);
                position += value.length();
            }

            void writeInt8At(int index, int8_t value);

            void writeInt16At(int index, int16_t value);

            void writeInt32At(int index, int32_t value);

            void writeInt64At(int index, int64_t value);

            void writeFloatAt(int index, float value);

            void writeDoubleAt(int index, double value);

            void writeStringAt(int index, const std::string &value);
        };
    }
}
