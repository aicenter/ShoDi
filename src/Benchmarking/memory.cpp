#if defined(_MSC_VER)
	#include <Windows.h>
	#include <psapi.h>
#elif defined(__GNUC__)
	#include <sys/resource.h>
#endif

#include "memory.h"
// #include <cmath>
#include <stdexcept>

void Memory::init() {
	#if defined(__GNUC__)
		struct rusage usage;
		getrusage(RUSAGE_SELF, &usage);
		init_value = usage.ru_maxrss;
	#endif
}

unsigned long long Memory::get_max_memory_usage() {
	unsigned long long max_mem;

	#if defined(_MSC_VER)
		PROCESS_MEMORY_COUNTERS pmc;
		K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		max_mem = pmc.PeakWorkingSetSize;
		max_mem = static_cast<long long>(std::round(static_cast<double>(max_mem) / 1024));
	#elif defined(__GNUC__)
		struct rusage usage;
		getrusage(RUSAGE_SELF, &usage);
		max_mem = usage.ru_maxrss;
		if (init_value == 0) {
			throw std::runtime_error("unable to compute allocated memory. Memory::init() had not been called before");
		}
		else if (max_mem == init_value) {
			// throw std::runtime_error("unable to compute allocated memory");
			max_mem = 0;
		}
	#endif

	return max_mem;
}

