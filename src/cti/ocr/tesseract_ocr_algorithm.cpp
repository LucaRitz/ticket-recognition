#include <include/ocr/ocr_algorithm.hpp>
#include "tesseract_ocr_algorithm.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/bounding_box.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include "../timer/timer.hpp"
#include <numeric>
#include <algorithm>

using namespace cv;

using std::string;

cti::impl::TesseractOcrAlgorithm::TesseractOcrAlgorithm(const string& language) {
    this->ocr->Init(nullptr, language.c_str(), tesseract::OEM_DEFAULT);
    this->ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

string cti::impl::TesseractOcrAlgorithm::read(
        const cti::TicketImage& image, const Ticket& ticket, const cti::BoundingBox& boundingBox) const {

    string text;

    int processTime = cti::Timer::timed([this, &ticket, &image, &boundingBox, &text] () {

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
        Mat dilationElement = getStructuringElement(MorphShapes::MORPH_RECT, Size(3, 3));
        morphologyEx(grayscale, dilated, MORPH_DILATE, dilationElement, cv::Point{-1,-1}, 5, BORDER_REPLICATE);

        Mat blurred;
        blur(dilated, blurred, Size{ 5, 5 });

        Mat subtracted;
        subtract(blurred, grayscale, subtracted);

        Mat binary_subtracted;
        threshold(subtracted, binary_subtracted, 0, 255, ThresholdTypes::THRESH_OTSU);

        Mat inverted_binary_subtracted;
        bitwise_not(binary_subtracted, inverted_binary_subtracted);

        // Floodfill white from all four corners in order to prevent ocr issues that are caused by borders in the
        // Ticket's TicketImage around text regions.
        Mat floodFilled = inverted_binary_subtracted.clone();
        const Scalar& white = Scalar{255, 255, 255};
        floodFill(floodFilled, cv::Point { 0, 0 }, white);
        floodFill(floodFilled, cv::Point { 0, floodFilled.rows -1 }, white);
        floodFill(floodFilled, cv::Point { floodFilled.cols -1, 0 }, white);
        floodFill(floodFilled, cv::Point { floodFilled.cols -1, floodFilled.rows -1 }, white);

        // Draw white border around section border in order to prevent ocr issues that are caused by borders in the
        // Ticket's TicketImage around text regions.
        Rect whiteBorderRect {0, 0, floodFilled.cols, floodFilled.rows};
        rectangle(floodFilled, whiteBorderRect, Scalar(255, 255, 255), 2);

        this->ocr->SetImage(floodFilled.data, floodFilled.cols, floodFilled.rows, floodFilled.channels(), floodFilled.step1());

        text = string(this->ocr->GetUTF8Text());

    #ifdef CTI_DEBUG
        std::vector<Mat> images = {
                section,
                grayscale,
                dilated,
                blurred,
                subtracted,
                binary_subtracted,
                inverted_binary_subtracted,
                floodFilled
        };

        int space = 10;
        int y = 0;
        int rows = std::accumulate(images.begin(), images.end(), 0, [&space](long count, Mat& c) {
            return count + c.rows + space;
        });
        int cols = std::max_element(images.begin(), images.end(), [](const Mat &a, const Mat &b) {
            return a.cols < b.cols;
        })->cols;
        Mat debug {rows, cols, CV_8UC3, white};

        for(auto& img : images) {

            Mat debugImg;
            if(img.type() == CV_8UC3) {
                debugImg = img.clone();
            } else {
                cvtColor(img, debugImg, COLOR_GRAY2BGR);
            }
            debugImg.copyTo(debug(cv::Rect{0, y, debugImg.cols, debugImg.rows}));
            y += debugImg.rows + space;
        }

        cv::imshow("debug", debug);
        waitKey();
    #endif
    });

#ifdef CTI_DEBUG
    std::cout << "  " << "Ticket: " << ticket.name() << " TIME: " << processTime << "ms " << " TEXT: " << text << std::endl;
#endif

    return text;
}
