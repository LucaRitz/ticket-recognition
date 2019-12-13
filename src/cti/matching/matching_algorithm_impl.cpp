#include "matching_algorithm_impl.hpp"
#include "../timer/timer.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket_match.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using std::string;
using std::vector;

cti::impl::MatchingAlgorithmImpl::MatchingAlgorithmImpl(
        double ratioTestThreshold,
        double scoreTestThreshold,
        double scoreThreshold,
        cv::Ptr<cv::Feature2D> feature2d,
        cv::Ptr<cv::DescriptorMatcher> matcher) :
        ratioTestThreshold(ratioTestThreshold),
        scoreTestThreshold(scoreTestThreshold),
        scoreThreshold(scoreThreshold),
        feature2d(std::move(feature2d)),
        matcher(std::move(matcher)) {

}

void cti::impl::MatchingAlgorithmImpl::train(const cti::Ticket& ticketTemplate) {
    const string name = ticketTemplate.name();

    const TicketImage& image = ticketTemplate.image();
    const Mat templateImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

    auto* keypoints = new vector<KeyPoint>();
    this->_keypoints[name] = keypoints;
    this->feature2d->detect(templateImage, *keypoints);

    auto descriptors = new Mat;
    this->_descriptors[name] = descriptors;
    this->feature2d->compute(templateImage, *keypoints, *descriptors);

    this->_trained.push_back(&ticketTemplate);
}

void cti::impl::MatchingAlgorithmImpl::train(const vector<const cti::Ticket*>& ticketTemplates) {
    for(auto &ticket : ticketTemplates) {
        this->train(*ticket);
    }
}

void cti::impl::MatchingAlgorithmImpl::untrain(const cti::Ticket& ticket) {
    // TODO: implement
}

vector<cti::TicketMatch> cti::impl::MatchingAlgorithmImpl::execute(const cti::TicketImage& ticketImage) {

    const Mat inputImage = Mat(ticketImage.height(), ticketImage.width(), CV_8UC(ticketImage.bytesPerPixel()), ticketImage.image());

    vector<KeyPoint> inputKeypoints;
    int keypointTime = cti::Timer::timed([this, &inputImage, &inputKeypoints] () {
        feature2d->detect(inputImage, inputKeypoints);
    });

    Mat inputDescriptors;
    int descriptorTime = cti::Timer::timed([this, &inputImage, &inputKeypoints, &inputDescriptors] () {
        feature2d->compute(inputImage, inputKeypoints, inputDescriptors);
    });

    auto matches = vector<cti::TicketMatch>();
    for(const auto ticketTemplate : this->_trained) { // TODO: improve efficiency of search

        string name = ticketTemplate->name();
        auto& templateKeypoints = *this->_keypoints.at(name);
        auto& templateDescriptors = *this->_descriptors.at(name);

        vector<vector<DMatch>> knnMatches;
        int knnTime = cti::Timer::timed([this, &inputDescriptors, &templateDescriptors, &knnMatches] () {
            this->matcher->knnMatch(inputDescriptors, templateDescriptors, knnMatches, 2);
        });

        vector<DMatch> bestMatches = ratioTest(knnMatches, ratioTestThreshold);

        if(bestMatches.size() < 4) {
            // Cannot find homography with less than 4 points
            // TODO: Requiring at least 4 points may not make sense, maybe require more points?
            // NO MATCH
            continue;
        }

        vector<Point2f> templatePoints;
        vector<Point2f> inputPoints;
        for (auto &m : bestMatches) {
            templatePoints.push_back(templateKeypoints[m.trainIdx].pt);
            inputPoints.push_back(inputKeypoints[m.queryIdx].pt);
        }

        Mat homography;
        Mat inlierMask;
        int homographyTime = cti::Timer::timed([this, &inputPoints, &templatePoints, &homography, &inlierMask] () {
            homography = findHomography(inputPoints, templatePoints, RANSAC, 3, inlierMask); // TODO: tweak parameter
        });

        bool matched = true;

        if(homography.empty()) {
            matched = false;
        }

        // See: https://stackoverflow.com/questions/10972438/detecting-garbage-homographies-from-findhomography-in-opencv
        const double HOMOGRAPHY_DETERMINANT_THRESHOLD = 0.001;
        double determinant = homography.empty() ? 0.0 : cv::determinant(homography);
        if(determinant < HOMOGRAPHY_DETERMINANT_THRESHOLD && determinant > -HOMOGRAPHY_DETERMINANT_THRESHOLD) {
//            matched = false;
        }

        int numberOfInliers = 0;
        if(!homography.empty()) {
            for (size_t i = 0; i < bestMatches.size(); i++) {
                uchar inlier = inlierMask.at<uchar>(i);
                if(inlier) {
                    numberOfInliers++;
                }
            }
        }
        double inliersRatio = (double)numberOfInliers / bestMatches.size();

        unsigned int numberOfMatches = bestMatches.size();
        unsigned int numberOfPotentialMatches = knnMatches.size();
        double matchesRatio = (double)numberOfMatches / (double)numberOfPotentialMatches;

        double score = matched ? inliersRatio * 10000 * matchesRatio : 0; // TODO: improve score

        int totalTime = keypointTime + descriptorTime + knnTime + homographyTime;
        std::cout << "SCORE: " << score << " "
                  << "TEMPLATE: " << name << " "
                  << "MATCHING-TEST: score=" << matchesRatio << " (" << numberOfMatches << "/" << numberOfPotentialMatches << ") "
                  << "HOMOGRAPHY: determinant=" << determinant << " inliers ratio=" << inliersRatio << " inliers=" << numberOfInliers << " outliers=" << (bestMatches.size() - numberOfInliers) << " "
                  << "TIME: " << "total=" << totalTime << "ms " << "keypoints=" << keypointTime << "ms " << "descriptors=" << descriptorTime << "ms " << " knn=" << knnTime << "ms " << " homography=" << homographyTime << "ms "
                  << "Thresholds: ratio-test=" << ratioTestThreshold << " " << " score=" << scoreThreshold << " " << " score-test=" << scoreTestThreshold << " "
                  << "KEYPOINTS: template=" << templateKeypoints.size() << " input=" << inputKeypoints.size() << " "
                  << "DESCRIPTORS: template=" << templateDescriptors.size() << " input=" << inputDescriptors.size() << " "
                  << std::endl;

        if(!matched) {
            // NO MATCH
            continue;
        }

        matches.emplace_back(TicketMatch{*ticketTemplate, score});
    }
    return matches;
}

std::pair<cti::TicketMatch*, cti::TicketMatch*> cti::impl::MatchingAlgorithmImpl::findBestTwoMatches(
        vector<cti::TicketMatch>& matches) const {

    cti::TicketMatch* bestMatch = nullptr;
    cti::TicketMatch* secondBestMatch = nullptr;
    for(auto& match : matches) {
        if(bestMatch == nullptr || match.score() > bestMatch->score()) {
            secondBestMatch = bestMatch;
            bestMatch = &match;
        } else if(secondBestMatch == nullptr || match.score() > secondBestMatch->score()){
            secondBestMatch = &match;
        }
    }
    return std::make_pair(bestMatch, secondBestMatch);
}

bool cti::impl::MatchingAlgorithmImpl::isBestMatchGood(
        cti::TicketMatch* bestMatch, cti::TicketMatch* secondBestMatch) const {

    if(bestMatch != nullptr && secondBestMatch != nullptr) {
        // Best match must be a good match
        if(bestMatch->score() >= scoreThreshold) {
            // Best match must be considerably better than the second best match
            if(bestMatch->score() * scoreTestThreshold >= secondBestMatch->score()) {
                return true;
            }
        }
    }
    return false;
}

const optional<cti::TicketMatch> cti::impl::MatchingAlgorithmImpl::select(vector<cti::TicketMatch>& matches) const {

    if(matches.size() > 1) {
        auto bestTwo = findBestTwoMatches(matches);
        if(isBestMatchGood(bestTwo.first, bestTwo.second)) {
            return std::make_optional(*bestTwo.first);
        }
        return std::nullopt;
    }

    if(matches.size() == 1) {
        // Match must be a good match
        if(matches.at(0).score() >= scoreThreshold) {
            return std::make_optional(matches.at(0));
        }
        return std::nullopt;
    }

    return std::nullopt;
}

vector<cv::DMatch> cti::impl::MatchingAlgorithmImpl::ratioTest(
        const vector<vector<cv::DMatch>>& knnMatches, double threshold) const {

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
