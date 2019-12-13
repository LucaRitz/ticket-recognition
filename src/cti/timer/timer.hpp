#pragma once

#include <chrono>

namespace cti {

    class Timer {
    public:
        template<typename Functor>
        inline static int timed(const Functor& fn) {
            #ifdef CTI_DEBUG
                cti::Timer timer;
                timer.start();
                fn();
                timer.stop();
                return timer.duration();
            #else
                fn();
                return 0;
            #endif
        }
        void start();
        void stop();
        int duration();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        int _duration = 0;
    };
}