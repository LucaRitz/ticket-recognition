#include "timer.hpp"

using namespace std::chrono;
using std::chrono::high_resolution_clock;

void cti::Timer::start() {
    startTime = high_resolution_clock::now();
}

void cti::Timer::stop() {
    auto endTime = high_resolution_clock::now();
    _duration = (endTime - startTime) / milliseconds(1);
}

int cti::Timer::duration() {
    return _duration;
}
