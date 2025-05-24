#if defined(_MSC_VER)
	#include <Windows.h>
	#include <psapi.h>
#elif defined(__GNUC__)
	#include <sys/resource.h>
#endif

#include "memory.h"
#include <cmath>

unsigned long long get_max_memory_usage() {
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
	#endif

	return max_mem;
}

