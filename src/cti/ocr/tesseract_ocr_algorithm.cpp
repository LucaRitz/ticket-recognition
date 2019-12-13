#include <include/ocr/ocr_algorithm.hpp>
#include "tesseract_ocr_algorithm.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

using namespace cv;

using std::string;

cti::impl::TesseractOcrAlgorithm::TesseractOcrAlgorithm(const string& language) {
    this->ocr->Init(nullptr, language.c_str(), tesseract::OEM_DEFAULT);
    this->ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

string cti::impl::TesseractOcrAlgorithm::read(
        const cti::TicketImage& image, const Ticket& ticket, const cti::BoundingBox& boundingBox) const {

    const TicketImage& templateTicketImage = ticket.image();
    const Mat templateImage = Mat(templateTicketImage.height(), templateTicketImage.width(), CV_8UC(templateTicketImage.bytesPerPixel()), templateTicketImage.image());
    const Mat inputImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

    Mat section = inputImage (
            Range(boundingBox.topLeft().y(), boundingBox.bottomRight().y()),
            Range(boundingBox.topLeft().x(), boundingBox.bottomRight().x())
    );

    this->ocr->SetImage(section.data, section.cols, section.rows, section.channels(), section.step);

    string text = string(this->ocr->GetUTF8Text());

    return text;
}
