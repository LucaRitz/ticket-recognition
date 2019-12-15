#include <include/ocr/ocr_algorithm.hpp>
#include "tesseract_ocr_algorithm.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/bounding_box.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include "../timer/timer.hpp"

using namespace cv;

using std::string;

cti::impl::TesseractOcrAlgorithm::TesseractOcrAlgorithm(const string& language) {
    this->ocr->Init(nullptr, language.c_str(), tesseract::OEM_DEFAULT);
    this->ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

string cti::impl::TesseractOcrAlgorithm::read(
        const cti::TicketImage& image, const Ticket& ticket, const cti::BoundingBox& boundingBox) const {

    string text;

    int time = cti::Timer::timed([this, &ticket, &image, &boundingBox, &text] () {

        const TicketImage& templateTicketImage = ticket.image();
        const Mat templateImage = Mat(templateTicketImage.height(), templateTicketImage.width(), CV_8UC(templateTicketImage.bytesPerPixel()), templateTicketImage.image());
        const Mat inputImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

        Mat section = inputImage (
                Range(boundingBox.topLeft().y(), boundingBox.bottomRight().y()),
                Range(boundingBox.topLeft().x(), boundingBox.bottomRight().x())
        );

        Mat grayscale;
        cvtColor(section, grayscale, COLOR_BGR2GRAY);

        Mat dilated;
        Mat element = getStructuringElement(MorphShapes::MORPH_RECT, Size(9, 9));
        morphologyEx(grayscale, dilated, MORPH_DILATE, element);

        Mat blurred;
        blur(dilated, blurred, Size{ 5, 5 });

        Mat subtracted;
        subtract(blurred, grayscale, subtracted);

        Mat binary_subtracted;
        threshold(subtracted, binary_subtracted, 0, 255, ThresholdTypes::THRESH_OTSU);

        Mat binary;
        bitwise_not(binary_subtracted, binary);

        this->ocr->SetImage(binary.data, binary.cols, binary.rows, binary.channels(), binary.step1());

        text = string(this->ocr->GetUTF8Text());
    });

    #ifdef CTI_DEBUG
        std::cout << "  " << "Ticket: " << ticket.name() << " TIME: " << time << "ms " << " TEXT: " << text << std::endl;
    #endif

    return text;
}
