#include <string>
#include <some/someClass.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <iostream>

using namespace std;
using namespace cv;
using namespace xfeatures2d;

void display(char *imageName1) {
    string outText;
    string imPath = imageName1;
    string imageName2 = imageName1;

    // Create Tesseract object
    cout << imageName1;
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    // Initialize tesseract to use English (eng) and the LSTM OCR engine.
    ocr->Init(NULL, "deu", tesseract::OEM_LSTM_ONLY);

    // Set Page segmentation mode to PSM_AUTO (3)
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    // Open input image using OpenCV
    Mat im = cv::imread(imPath, IMREAD_COLOR);

    // Set image data
    ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);

    // Run Tesseract OCR on image
    outText = string(ocr->GetUTF8Text());

    // print recognized text
    cout << outText << endl; // Destroy used object and release memory ocr->End();

    ocr->End();

    Ptr<SIFT> sift = cv::xfeatures2d::SIFT::create();
    const cv::Mat img1 = cv::imread(imageName1, 0);
    const cv::Mat img2 = cv::imread(imageName2, 0);

    //-- Step 1: Detect the keypoints:
    std::vector<KeyPoint> keypoints1, keypoints2;
    sift->detect(img1, keypoints1);
    sift->detect(img2, keypoints2);

    cv::Mat output;
    cv::drawKeypoints(img1, keypoints1, output);
    cv::imwrite("sift_result.jpg", output);

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    sift->compute(img1, keypoints1, descriptors_1);
    sift->compute(img2, keypoints2, descriptors_2);

    //-- Step 3: Matching descriptor vectors using BFMatcher :
    BFMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);
    for(auto x : matches) {
        cout << "Distance is: " << x.distance << endl;
    }
}

int test(int a) {
    return a - 4;
}
