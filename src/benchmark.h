#pragma once

#include <chrono>
#include <type_traits>
#include <functional>


template<class R, typename T>
struct benchmark_result {
	R return_value;
	T computational_time;

	benchmark_result(R&& return_value_par, const T& computational_time_par)
		: return_value(std::move(return_value_par)),
		computational_time(computational_time_par) {
	}

	/**
	 * @brief Returns the computational time in [T] calling T.count()
	 * @return computational time
	*/
	unsigned long count() {
		return static_cast<long>(computational_time.count());
	}
};

template<
	typename T = std::chrono::milliseconds,
	typename F,
	typename ...A,
	typename R = std::invoke_result_t<F, A...>,
	std::enable_if_t<std::is_void_v<R>, int> = 0
>
static T benchmark(F&& func, A&&... args)
{
	const auto start = std::chrono::high_resolution_clock::now();
	std::invoke(std::forward<F>(func), std::forward<A>(args)...);
	return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - start);
}

template<
	typename T = std::chrono::milliseconds,
	typename F,
	typename ...A,
	typename R = std::invoke_result_t<F, A...>,
	std::enable_if_t<!std::is_void_v<R>, int> = 0
>
static benchmark_result<R, T> benchmark(F&& func, A&&... args)
{
	const auto start = std::chrono::high_resolution_clock::now();

	return benchmark_result{
		std::invoke(std::forward<F>(func), std::forward<A>(args)...),
		std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - start)
	};
}
