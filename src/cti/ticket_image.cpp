#include <include/ticket_image.hpp>

#include <opencv2/opencv.hpp>

cti::TicketImage::TicketImage(const string& imagePath) : _width(0), _height(0), _bytes_per_pixel(3), _bytes_per_line(0) {
    const cv::Mat image = cv::imread(imagePath);
    // size_t dataSize = image.total() * image.step.p[image.dims - 1];
    size_t dataSize = image.total() * image.elemSize();

    this->_width = image.size().width;
    this->_height = image.size().height;
    this->_bytes_per_pixel = image.channels();
    this->_bytes_per_line = image.step1();

    this->_image = new unsigned char[dataSize];
    memcpy(this->_image, image.data, dataSize);
}