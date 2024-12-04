/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */
#if defined(_MSC_VER)
	#include <Windows.h>
	#include <psapi.h>
#elif defined(__GNUC__)
    #include <sys/resource.h>
    #include <cstdlib>
    #include <ctime>
    #include <iostream>
    #include <cstdint>
    #include <vector>
#endif

#include "memory.h"
#include <cmath>
#include <stdexcept>

void Memory::init() {
#if defined(__GNUC__)
    struct rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    init_value = usage.ru_maxrss;

    // fill random_vector so that it allocates 128 MiB (131072 B)
    for (size_t i = 0; static_cast<double>(i) < std::pow(2.0, 24); i++) {
        random_vector.push_back(rand());
    }
#endif
}

unsigned long long Memory::get_max_memory_usage() {
    unsigned long long max_mem;

#if defined(_MSC_VER)
	PROCESS_MEMORY_COUNTERS pmc;
	K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	max_mem = pmc.PeakWorkingSetSize;
	max_mem = static_cast<long long>(std::round(static_cast<double>(max_mem) / 1024));

    return max_mem;

#elif defined(__GNUC__)
    struct rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    max_mem = usage.ru_maxrss;
    if (init_value == 0) {
        throw std::runtime_error("Unable to compute allocated memory. Memory::init() had not been called before.");
    }
    if (max_mem == init_value) {
        /* This would happen if the process did not allocate more memory than its parent (typically Python
           interpreter). If this happens, random_vector must be filled with even more data. */
        std::cerr << "Unable to compute allocated memory." << std::endl;
        max_mem = 0;
    }
    auto index = rand() % random_vector.size();
    std::cerr << "please ignore this random number: " << random_vector[index] << std::endl;

    return max_mem - 131072;
#endif
}
