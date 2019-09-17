#include "../some/someClass.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>

using std::cout;
using std::endl;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }
    display(argv[1]);

    const cv::Mat input = cv::imread("input.jpg", 0); //Load as grayscale

    cv::Ptr<cv::SiftFeatureDetector> detector = cv::SiftFeatureDetector::create();
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(input, keypoints);

    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);

    return 0;
}