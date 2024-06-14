#include "Time.h"

gde::Time::Time()
{
    using clock = std::chrono::high_resolution_clock;
    curr_time = clock::now();
    prev_time = curr_time;
    curr_ns = std::chrono::nanoseconds(0);
}

void gde::Time::TickFixed(std::function<void(double)> update_callback)
{
    using clock = std::chrono::high_resolution_clock;

    curr_time = clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
    prev_time = curr_time;

    curr_ns += dur;

    while (curr_ns >= timestep) {

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestep);
        auto seconds = (double)ms.count() / 1000;
        update_callback(seconds);

        curr_ns -= timestep;
    }
}
