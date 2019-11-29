#include <include/ocr/ocr_algorithm.hpp>
#include <include/ocr/ocr_algorithms.hpp>
#include "tesseract_ocr_algorithm.hpp"

using std::string;
using std::shared_ptr;

cti::OcrAlgorithms::OcrAlgorithms() = default;

shared_ptr<cti::OcrAlgorithm> cti::OcrAlgorithms::tesseract(const string& language) {
    return std::make_shared<cti::impl::TesseractOcrAlgorithm>(
            language
    );
}
