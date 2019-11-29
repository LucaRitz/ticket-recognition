#pragma once

#include <include/dllmacro.h>
#include <chrono>

namespace cti {

    class EXPORT Timer {
    public:
        template<typename Functor>
        static int timed(const Functor& fn)  {
            cti::Timer timer;
            timer.start();
            fn();
            timer.stop();
            return timer.duration();
        }
        void start();
        void stop();
        int duration();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        int _duration = 0;
    };
}