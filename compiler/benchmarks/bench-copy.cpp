//
// Created by kiva on 2019/8/25.
//
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <chrono>

using byte = uint8_t;

void randomFill(byte *array, size_t length) {
    for (int i = 0; i < length; ++i) {
        array[i] = (uint8_t) i;
    }
}

void benchmark(const char *tag, void(*func)(byte *, byte *, size_t), byte *from, byte *to, size_t count) {
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 1000; ++i) {
        func(from, to, count);
    }
    auto end = std::chrono::system_clock::now();
    auto cost = end - start;
    printf("benchmark %s: %lld\n", tag, cost.count());
}

void copyUsingDuffsDevice(byte *from, byte *to, size_t count) {
    size_t n = (count + 7) / 8;
    switch (count % 8) {
        case 0:
            do {
                *to++ = *from++;
                case 7:
                    *to++ = *from++;
                case 6:
                    *to++ = *from++;
                case 5:
                    *to++ = *from++;
                case 4:
                    *to++ = *from++;
                case 3:
                    *to++ = *from++;
                case 2:
                    *to++ = *from++;
                case 1:
                    *to++ = *from++;
            } while (--n > 0);
    }
}

void copyUsingMemcpy(byte *from, byte *to, size_t count) {
    memcpy(to, from, count);
}

int main() {
    int maxPower = 10;

    byte *array1 = new byte[1 << maxPower];
    byte *array2 = new byte[1 << maxPower];

    for (int i = 0; i < maxPower; ++i) {
        size_t count = 1 << i;
        printf("Test for copy %zd bytes\n", count);
        benchmark("duff's device", copyUsingDuffsDevice, array1, array2, count);
        benchmark("memcp", copyUsingMemcpy, array1, array2, count);
        printf("\n");
    }
}
