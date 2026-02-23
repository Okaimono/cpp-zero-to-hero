#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <x86intrin.h>

class Profiler {
public:
    template<typename T>
    static void profile(T func)
    {
        unsigned long long startCycles = __rdtsc();
        auto start = std::chrono::high_resolution_clock::now();

        func();

        auto end = std::chrono::high_resolution_clock::now();
        unsigned long long endCycles = __rdtsc();

        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        unsigned long long cycles = endCycles - startCycles;

        std::cout << micro.count() << ", ";
    }
};

#endif