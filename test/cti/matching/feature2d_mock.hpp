#pragma once

#include <gmock/gmock.h>
#include <opencv2/features2d.hpp>

namespace cti::mock {
    class Feature2dMock : public cv::Feature2D {
    public:
        MOCK_METHOD0(clear, void());
        MOCK_METHOD3(detect, void(cv::InputArray, std::vector<cv::KeyPoint>&, cv::InputArray));
        MOCK_METHOD3(compute, void(cv::InputArray, std::vector<cv::KeyPoint>&, cv::OutputArray));
    };
}