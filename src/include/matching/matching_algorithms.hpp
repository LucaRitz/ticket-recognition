#pragma once

#include <include/dllmacro.h>
#include <include/matching/matching_algorithm.hpp>
#include <memory>

using std::shared_ptr;

namespace cti {

    class MatchingAlgorithm;

    class EXPORT MatchingAlgorithms {
    public:
        MatchingAlgorithms();
        static shared_ptr<MatchingAlgorithm> orb(); // TODO: tweak parameter
        static shared_ptr<MatchingAlgorithm> sift(); // TODO: tweak parameter
    };
}