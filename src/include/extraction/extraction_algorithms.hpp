#pragma once

#include <include/dllmacro.h>
#include <include/extraction/extraction_algorithm.hpp>
#include <memory>

using std::shared_ptr;

namespace cti {

    class ExtractionAlgorithm;

    class EXPORT ExtractionAlgorithms {
    public:
        ExtractionAlgorithms();
        static shared_ptr<ExtractionAlgorithm> orb(); // TODO: tweak parameter
        static shared_ptr<ExtractionAlgorithm> sift(); // TODO: tweak parameter
    };
}