#pragma once

#include <include/dllmacro.h>
#include "ocr_algorithm.hpp"
#include <string>
#include <memory>

using std::shared_ptr;
using std::string;

namespace cti {

    class OcrAlgorithm;

    class EXPORT OcrAlgorithms {
    public:
        OcrAlgorithms();
        static shared_ptr<OcrAlgorithm> tesseract(const string& language); // TODO: tweak parameter
    };
}