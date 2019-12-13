#pragma once

#include <include/dllmacro.h>
#include <include/extraction/extraction_algorithm.hpp>
#include <include/ocr/ocr_algorithm.hpp>
#include <include/ocr/ocr_algorithms.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <include/text.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>

namespace cti::impl {

    class EXPORT ExtractionAlgorithmImpl : public ExtractionAlgorithm {
    public:
        explicit ExtractionAlgorithmImpl(
                double ratioTestThreshold = 0.7,
                cv::Ptr<cv::Feature2D> feature2d = cv::xfeatures2d::SIFT::create(),
                cv::Ptr<cv::DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(cv::FlannBasedMatcher {}),
                std::unique_ptr<cti::OcrAlgorithm> ocr = cti::OcrAlgorithms::tesseract("eng")
                );
        cti::TicketImage* normalize(const Ticket&, const TicketImage&) const override;
        Metadata read(const Ticket&, TicketImage&) const override;
    private:
        const cv::Ptr<cv::Feature2D> feature2d;
        const cv::Ptr<cv::DescriptorMatcher> matcher;
        const std::unique_ptr<cti::OcrAlgorithm> ocr;

        std::string extractText(const Ticket& ticket, const cti::Text* textDefinition, const cti::TicketImage& inputImage) const;
        const cti::BoundingBox clampToImage(const cti::BoundingBox& boundingBox, const cti::TicketImage& inputImage) const;
        void postProcessText(string& text) const;
        cti::TicketImage* matToTicketImage(const cv::Mat& image) const;
        std::vector<cv::DMatch> ratioTest(const std::vector<std::vector<cv::DMatch>>& knnMatches, double threshold) const;
        double ratioTestThreshold;
    };
}
