#include "Time.h"

#include <iostream>

gde::Time::Time()
{
    using clock = std::chrono::high_resolution_clock;
    curr_time = clock::now();
    prev_time = curr_time;
    curr_ns = std::chrono::nanoseconds(0);
}

bool gde::Time::TickFixed(double* seconds)
{
    using clock = std::chrono::high_resolution_clock;

    curr_time = clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
    prev_time = curr_time;

    curr_ns += dur;
    if (curr_ns >= timestep) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
        curr_ns -= curr_ns;

        if (seconds != nullptr) {
            *seconds = (double)ms.count() / 1000;
        }

        return true;
    }

    return false;
}
