#include <include/matching/matching_algorithm.hpp>
#include <include/matching/matching_algorithms.hpp>
#include "matching_algorithm_impl.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

cti::MatchingAlgorithms::MatchingAlgorithms() = default;

std::shared_ptr<cti::MatchingAlgorithm> cti::MatchingAlgorithms::orb() {
    return std::make_shared<cti::impl::MatchingAlgorithmImpl>(
            0.7,
            0.5,
            200.0,
            cv::ORB::create(),
            cv::makePtr<cv::FlannBasedMatcher>(
                    cv::FlannBasedMatcher{cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2)})
    );
}

std::shared_ptr<cti::MatchingAlgorithm> cti::MatchingAlgorithms::sift() {
    return std::make_shared<cti::impl::MatchingAlgorithmImpl>(
            0.7,
            0.5,
            1000.0,
            cv::xfeatures2d::SIFT::create(),
            cv::makePtr<cv::FlannBasedMatcher>(cv::FlannBasedMatcher{})
    );
}
