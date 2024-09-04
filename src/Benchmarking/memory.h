#pragma once
/**
 * @brief Get the maximum memory usage of the program in kibytes (KiB)
 * @return the maximum memory usage of the program in kibytes (KiB)
 */

class Memory {
    unsigned long long init_value = 0;

    public:
    void init();
    unsigned long long get_max_memory_usage();
};
