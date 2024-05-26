#pragma once

#include <chrono>
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(32ms);

namespace gde {
	class Time {
	private:
		std::chrono::steady_clock::time_point curr_time;
		std::chrono::steady_clock::time_point prev_time;
		std::chrono::nanoseconds curr_ns;
	public:
		Time();
		/// <summary>
		/// Returns true if it is time to do a fixed update. Can only return true once per fixed update.
		/// </summary>
		bool TickFixed(double* seconds = nullptr);
	};
}