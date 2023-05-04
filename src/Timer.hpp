#pragma once

#include <Arduino.h>

namespace mtrn3100 {

class Timer {
public:
    Timer() {}

    // Duration in milliseconds.
    void time(double const wait_time) {
        duration = wait_time;
        start = millis();
    }

    bool expired() const { return millis() > start + duration; }

    void wait(double const wait_time) {
        time(wait_time);
        while (!expired()) {
        }
    }

    // friend std::ostream& operator<<(std::ostream& os, Timer const& timer) {
    //     os << "Timer - ";
    //     os << "start: " << timer.start << " - ";
    //     os << "duration: " << timer.duration << " - ";
    //     os << "current: " << timer.mRobot.getTime();
    //     return os;
    // }

private:
    double start;
    double duration;
};

}  // namespace mtrn3100