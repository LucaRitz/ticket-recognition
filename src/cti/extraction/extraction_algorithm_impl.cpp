#include "extraction_algorithm_impl.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <include/metadata.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "../timer/timer.hpp"

using namespace cv;

cti::impl::ExtractionAlgorithmImpl::ExtractionAlgorithmImpl(
        double ratioTestThreshold,
        cv::Ptr<cv::Feature2D> feature2d,
        cv::Ptr<cv::DescriptorMatcher> matcher,
        std::unique_ptr<cti::OcrAlgorithm> ocr) :
        ratioTestThreshold(ratioTestThreshold),
        feature2d(std::move(feature2d)),
        matcher(std::move(matcher)),
        ocr(std::move(ocr)) {

}

cti::TicketImage* cti::impl::ExtractionAlgorithmImpl::normalize(
        const cti::Ticket &ticketTemplate, const cti::TicketImage &ticketImage) const {

    const TicketImage &image = ticketTemplate.image();
    const Mat templateImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

    const Mat inputImage = Mat(ticketImage.height(), ticketImage.width(), CV_8UC(ticketImage.bytesPerPixel()),
                               ticketImage.image());

    vector<KeyPoint> keypoints;
    vector<KeyPoint> inputKeypoints;
    int keypointTime = cti::Timer::timed([this, templateImage, inputImage, &keypoints, &inputKeypoints] () {
        feature2d->detect(templateImage, keypoints);
        feature2d->detect(inputImage, inputKeypoints);
    });

    Mat templateDescriptors;
    Mat inputDescriptors;
    int descriptorTime = cti::Timer::timed([this, templateImage, inputImage, &keypoints, &inputKeypoints, &templateDescriptors, &inputDescriptors] () {
        feature2d->compute(inputImage, inputKeypoints, inputDescriptors);
        feature2d->compute(templateImage, keypoints, templateDescriptors);
    });

    vector<vector<DMatch>> knnMatches;
    int knnTime = cti::Timer::timed([this, inputDescriptors, templateDescriptors, &knnMatches] () {
        matcher->knnMatch(inputDescriptors, templateDescriptors, knnMatches, 2);
    });

    vector<DMatch> matches = ratioTest(knnMatches, ratioTestThreshold);

    if(matches.size() < 4) {
        // Cannot find homography with less than 4 points
        // TODO: throw error?
        return nullptr;
    }

    vector<Point2f> templatePoints;
    vector<Point2f> inputPoints;
    for (auto &m : matches) {
        templatePoints.push_back(keypoints[m.trainIdx].pt);
        inputPoints.push_back(inputKeypoints[m.queryIdx].pt);
    }

    Mat homography;
    int homographyTime = cti::Timer::timed([this, inputPoints, templatePoints, &homography] () {
        homography = findHomography(inputPoints, templatePoints, RANSAC); // TODO: tweak parameter
    });

    if (!homography.empty()) {

        Mat warped;
        int warpTime = cti::Timer::timed([this, &inputImage, &warped, homography, &ticketTemplate] () {
            warpPerspective(inputImage, warped, homography, inputImage.size());
        });

        int totalTime = keypointTime + descriptorTime + knnTime + homographyTime + warpTime;
        std::cout << "TOTAL TIME: " << totalTime << "ms "
                  << "KEYPOINTS: template=" << keypoints.size() << " input=" << inputKeypoints.size() << " time=" << keypointTime << "ms "
                  << "DESCRIPTORS: template=" << templateDescriptors.size() << " input=" << inputDescriptors.size() << " time=" << descriptorTime << "ms "
                  << "KNN: matches=" << knnMatches.size() << " time=" << knnTime << "ms "
                  << "RATIO-TEST: good=" << matches.size() << " threshold=" << ratioTestThreshold << " "
                  << "HOMOGRAPHY: determinant=" << cv::determinant(homography) << " time=" << homographyTime << "ms "
                  << "WARP: time=" << warpTime << "ms "
                  << std::endl;

        auto* normalizedImage = matToTicketImage(warped);
        return normalizedImage;

    } else {
        std::cout << "Unable to find homography" << std::endl;
        // TODO: throw error?
    }
    return nullptr;
}

cti::Metadata cti::impl::ExtractionAlgorithmImpl::read(
        const cti::Ticket &ticketTemplate, cti::TicketImage &ticketImage) const {

    const Mat inputImage = Mat(ticketImage.height(), ticketImage.width(), CV_8UC(ticketImage.bytesPerPixel()),
                               ticketImage.image());

    std::unordered_map<string, string> map;
    for(auto& textDefinition : ticketTemplate.texts()) {

        string text = this->extractText(ticketTemplate, textDefinition, ticketImage);
        if(!text.empty()) {
            map.insert({textDefinition->key(), text});
        }
    }
    return cti::Metadata(map);
}

const cti::BoundingBox cti::impl::ExtractionAlgorithmImpl::clampToImage(
        const cti::BoundingBox& boundingBox, const cti::TicketImage& inputImage) const {

    const cti::Point bottomRight = cti::Point {
            inputImage.width() > boundingBox.bottomRight().x() ? boundingBox.bottomRight().x() : inputImage.width(),
            inputImage.height() > boundingBox.bottomRight().y() ? boundingBox.bottomRight().y() : inputImage.height(),
    };

    const cti::BoundingBox clamped {
            boundingBox.topLeft(),
            bottomRight
    };

    return clamped;
}

string cti::impl::ExtractionAlgorithmImpl::extractText(
        const Ticket& ticket, const cti::Text* textDefinition, const cti::TicketImage& inputImage) const {

    const auto& boundingBox = this->clampToImage(textDefinition->boundingBox(), inputImage);

    string text = ocr->read(inputImage, ticket, boundingBox);
    postProcessText(text);

    return text;
}

void cti::impl::ExtractionAlgorithmImpl::postProcessText(string& text) const {

    // Remove trailing newline that Tesseract adds
    if (!text.empty() && text[text.length() - 1] == '\n') {
        text.erase(text.length() - 1);
    }
}

cti::TicketImage* cti::impl::ExtractionAlgorithmImpl::matToTicketImage(const cv::Mat& image) const {

    size_t dataSize = image.total() * image.elemSize();
    auto* imageData = new unsigned char[dataSize];
    memcpy(imageData, image.data, dataSize);

    auto* ticketImage = new TicketImage { image.size().width, image.size().height, image.channels(), image.step1(), imageData };
    return ticketImage;
}

std::vector<cv::DMatch> cti::impl::ExtractionAlgorithmImpl::ratioTest(
        const std::vector<std::vector<cv::DMatch>>& knnMatches, double threshold) const {

    vector<DMatch> matches;
    for (auto &match : knnMatches) {
        if(match.size() >= 2) {
            auto &first = match[0];
            auto &second = match[1];
            if (first.distance < threshold * second.distance) {
                matches.push_back(first);
            }
        }
    }
    return matches;
}
