#include <include/matching/matching_algorithm.hpp>
#include <include/matching/matching_algorithms.hpp>
#include <include/matching/matching_options.hpp>
#include "matching_algorithm_impl.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

std::unique_ptr<cti::MatchingAlgorithm> cti::MatchingAlgorithms::orb(const MatchingOptions& options) {
    return std::make_unique<cti::impl::MatchingAlgorithmImpl>(
            options.ratioTestThreshold(),
            options.scoreTestThreshold(),
            options.scoreThreshold(),
            cv::ORB::create(),
            cv::makePtr<cv::FlannBasedMatcher>(
                    cv::FlannBasedMatcher{cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2)})
    );
}

std::unique_ptr<cti::MatchingAlgorithm> cti::MatchingAlgorithms::sift(const MatchingOptions& options) {
    return std::make_unique<cti::impl::MatchingAlgorithmImpl>(
            options.ratioTestThreshold(),
            options.scoreTestThreshold(),
            options.scoreThreshold(),
            cv::xfeatures2d::SIFT::create(),
            cv::makePtr<cv::FlannBasedMatcher>(cv::FlannBasedMatcher{})
    );
}
