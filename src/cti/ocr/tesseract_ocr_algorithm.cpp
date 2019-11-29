#include <include/ocr/ocr_algorithm.hpp>
#include "tesseract_ocr_algorithm.hpp"
#include <include/ticket_image.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

using namespace cv;

using std::string;

cti::impl::TesseractOcrAlgorithm::TesseractOcrAlgorithm(const string& language) { // TODO: work on parameters
    this->ocr->Init(nullptr, language.c_str(), tesseract::OEM_TESSERACT_LSTM_COMBINED); // TODO: tweak parameter
    this->ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE); // TODO: tweak parameter
}

string cti::impl::TesseractOcrAlgorithm::read(const cti::TicketImage& image, const cti::BoundingBox& boundingBox) const {

    const Mat inputImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

    Mat section = inputImage (
            Range(boundingBox.topLeft().y(), boundingBox.bottomRight().y()),
            Range(boundingBox.topLeft().x(), boundingBox.bottomRight().x())
    );

    this->ocr->SetImage(section.data, section.cols, section.rows, section.channels(), section.step);

    string text = string(this->ocr->GetUTF8Text());

    return text;
}
