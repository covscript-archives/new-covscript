//
// Created by kiva on 2019/8/25.
//
#pragma once

#include <cstdio>
#include <chrono>

namespace cs {
    void benchmark(const char *tag, void(*func)()) {
        auto start = std::chrono::system_clock::now();
        for (int i = 0; i < 1000; ++i) {
            func();
        }
        auto end = std::chrono::system_clock::now();
        auto cost = end - start;
        printf("benchmark(1000 times call) %s: %lld\n", tag, cost.count());
    }
}
