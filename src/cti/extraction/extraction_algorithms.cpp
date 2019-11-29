#include <include/extraction/extraction_algorithm.hpp>
#include "extraction_algorithm_impl.hpp"
#include "cti/ocr/tesseract_ocr_algorithm.hpp"
#include <include/extraction/extraction_algorithms.hpp>
#include <include/ocr/ocr_algorithm.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;

cti::ExtractionAlgorithms::ExtractionAlgorithms() = default;

std::shared_ptr<cti::ExtractionAlgorithm> cti::ExtractionAlgorithms::orb() {
    return std::make_shared<cti::impl::ExtractionAlgorithmImpl>(
            0.7,
            cv::ORB::create(),
            cv::makePtr<cv::FlannBasedMatcher>(
                    cv::FlannBasedMatcher{cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2)}),
            cv::makePtr<cti::impl::TesseractOcrAlgorithm>(cti::impl::TesseractOcrAlgorithm{})
    );
}

std::shared_ptr<cti::ExtractionAlgorithm> cti::ExtractionAlgorithms::sift() {
    return std::make_shared<cti::impl::ExtractionAlgorithmImpl>(
            0.7,
            cv::xfeatures2d::SIFT::create(),
            cv::makePtr<cv::FlannBasedMatcher>(cv::FlannBasedMatcher{}),
            cv::makePtr<cti::impl::TesseractOcrAlgorithm>(cti::impl::TesseractOcrAlgorithm{})
    );
}
