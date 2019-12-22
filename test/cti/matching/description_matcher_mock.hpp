#pragma once

#include <gmock/gmock.h>
#include <opencv2/features2d.hpp>

namespace cti::mock {
    class DescriptionMatcherMock : public cv::DescriptorMatcher {
    public:
        MOCK_METHOD0(empty, bool());

        MOCK_METHOD0(clear, void());

        MOCK_METHOD1(add, void(cv::InputArrayOfArrays));

        MOCK_METHOD0(train, void());

        MOCK_METHOD3(knnMatch, void(cv::InputArray, std::vector<std::vector<cv::DMatch>>
                &, int));

        MOCK_CONST_METHOD0(isMaskSupported, bool());

        MOCK_CONST_METHOD1(clone, cv::Ptr<cv::DescriptorMatcher>(bool));

        MOCK_METHOD5(knnMatchImpl, void(cv::InputArray,
                std::vector<std::vector<cv::DMatch>>&, int, cv::InputArrayOfArrays, bool));

        MOCK_METHOD5(radiusMatchImpl, void(cv::InputArray,
                std::vector<std::vector<cv::DMatch>> &, float, cv::InputArrayOfArrays, bool));
    };
}