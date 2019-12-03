#pragma once

#include <include/dllmacro.h>
#include <include/ocr/ocr_algorithm.hpp>
#include <tesseract/baseapi.h>
#include <string>

using std::string;
using tesseract::TessBaseAPI;

namespace cti::impl {

    class EXPORT TesseractOcrAlgorithm : public OcrAlgorithm {
    public:
        explicit TesseractOcrAlgorithm(const string& language = "deu"); // TODO: work on parameters
        string read(const TicketImage& image, const BoundingBox& boundingBox) const override;
    private:
        TessBaseAPI* ocr = new TessBaseAPI();
    };
}