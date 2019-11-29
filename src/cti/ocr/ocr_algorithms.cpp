#include <include/ocr/ocr_algorithm.hpp>
#include <include/ocr/ocr_algorithms.hpp>
#include "tesseract_ocr_algorithm.hpp"

using std::string;
using std::unique_ptr;

cti::OcrAlgorithms::OcrAlgorithms() = default;

unique_ptr<cti::OcrAlgorithm> cti::OcrAlgorithms::tesseract(const string& language) {
    return std::make_unique<cti::impl::TesseractOcrAlgorithm>(
            language
    );
}
