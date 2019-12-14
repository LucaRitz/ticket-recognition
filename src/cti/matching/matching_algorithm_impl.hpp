#pragma once

#include <include/matching/matching_algorithm.hpp>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <opencv2/xfeatures2d/nonfree.hpp>

using std::string;
using std::vector;
using std::unordered_map;

namespace cti::impl {

    class MatchingAlgorithmImpl : public MatchingAlgorithm {
    public:
        explicit MatchingAlgorithmImpl(
                double ratioTestThreshold = 0.7,
                double scoreTestThreshold = 0.5,
                double scoreThreshold = 25.0,
                cv::Ptr<cv::Feature2D> feature2d = cv::xfeatures2d::SIFT::create(),
                cv::Ptr<cv::DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(cv::FlannBasedMatcher{})
        );
        void train(const Ticket &) override;
        void train(const vector<const Ticket *> &) override;
        void untrain(const Ticket& ticket) override;
        vector<TicketMatch> execute(const TicketImage &) override;
        const optional<TicketMatch> select(vector<TicketMatch> &) const override;

    private:
        double ratioTestThreshold;
        double scoreTestThreshold;
        double scoreThreshold;
        vector<const Ticket *> _trained;
        const cv::Ptr<cv::Feature2D> feature2d;
        const cv::Ptr<cv::DescriptorMatcher> matcher;
        unordered_map<string, vector<cv::KeyPoint> *> _keypoints;
        unordered_map<string, cv::Mat *> _descriptors;
        vector<cv::DMatch> ratioTest(const vector<vector<cv::DMatch>> &knnMatches,double threshold) const;
        bool isBestMatchGood(cti::TicketMatch *bestMatch, cti::TicketMatch *secondBestMatch) const;
        std::pair<cti::TicketMatch *, cti::TicketMatch *> findBestTwoMatches(vector<cti::TicketMatch> &matches) const;
    };
}