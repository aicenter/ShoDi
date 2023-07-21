#pragma once

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable: 4996)
	#pragma warning(disable: 4244)
	#pragma warning(disable: 4267)
#elif defined(__GNUC__)
	#pragma GCC diagnostic push
	//#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#elif defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <indicators/dynamic_progress.hpp>
#include <indicators/progress_bar.hpp>

#if defined(_MSC_VER)
	#pragma warning(pop)
#elif defined(__GNUC__)
	#pragma GCC diagnostic pop
#elif defined(__clang__)
	#pragma clang diagnostic pop
#endif


