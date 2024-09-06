#pragma once

#include <chrono>
#include <functional>
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(32ms);

namespace gde {
	class Time {
	private:
		std::chrono::steady_clock::time_point curr_time;
		std::chrono::steady_clock::time_point prev_time;
		double curr_ns;
	public:
		Time();
		double scale = 1;
		bool paused = false;
		void Reset();
		/// <summary>
		/// Returns true if it is time to do a fixed update. Can only return true once per fixed update.
		/// </summary>
		void TickFixed(std::function<void(double)> update_callback);
	};
}