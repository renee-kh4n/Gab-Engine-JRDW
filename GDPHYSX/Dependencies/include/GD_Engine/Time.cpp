#include "Time.h"

gde::Time::Time()
{
    using clock = std::chrono::high_resolution_clock;
    curr_time = clock::now();
    prev_time = curr_time;
    curr_ns = 0;
}

void gde::Time::Reset() {
    prev_time = curr_time;
}

void gde::Time::TickFixed(std::function<void(double)> update_callback)
{
    using clock = std::chrono::high_resolution_clock;

    curr_time = clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
    prev_time = curr_time;

    if (this->paused)
        return;

    curr_ns += (float)dur.count() * scale;

    while (curr_ns >= timestep.count()) {

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestep);
        auto seconds = (double)ms.count() / 1000;
        update_callback(seconds);

        curr_ns -= timestep.count();
    }
}
