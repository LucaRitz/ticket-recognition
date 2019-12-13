#include <include/ticket_image.hpp>

#include <opencv2/opencv.hpp>
#include <include/exception/cti_exception.hpp>

cti::TicketImage::TicketImage(const string& imagePath)
    : _width(0), _height(0), _bytes_per_pixel(3), _bytes_per_line(0) {

    const cv::Mat image = cv::imread(imagePath);

    if(image.data == nullptr) {
        throw CtiException { "Unable to load image using filepath '"+ imagePath +"'" };
    }

    size_t dataSize = image.total() * image.elemSize();

    this->_width = image.size().width;
    this->_height = image.size().height;
    this->_bytes_per_pixel = image.channels();
    this->_bytes_per_line = image.step1();

    this->_image = new unsigned char[dataSize];
    memcpy(this->_image, image.data, dataSize);
}

cti::TicketImage::TicketImage(
        const int width, const int height, const int bytes_per_pixel, const size_t bytes_per_line, unsigned char *image)
        : _width(width), _height(height), _bytes_per_pixel(bytes_per_pixel), _bytes_per_line(bytes_per_line),
          _image(image) {

    if(width <= 0) {
        throw CtiException { "TicketImage width must be greater than 0" };
    }

    if(height <= 0) {
        throw CtiException { "TicketImage height must be greater than 0" };
    }

    if(bytes_per_pixel <= 0) {
        throw CtiException { "TicketImage bytes_per_pixel must be greater than 0" };
    }

    if(bytes_per_line <= 0) {
        throw CtiException { "TicketImage bytes_per_line must be greater than 0" };
    }

    if(image == nullptr) {
        throw CtiException { "TicketImage image pointer must not be NULL" };
    }

}

cti::TicketImage::~TicketImage() {
    delete[] _image;
}

int cti::TicketImage::width() const {
    return _width;
}

int cti::TicketImage::height() const {
    return _height;
}

int cti::TicketImage::bytesPerPixel() const {
    return _bytes_per_pixel;
}

size_t cti::TicketImage::bytesPerLine() const {
    return _bytes_per_line;
}

unsigned char* cti::TicketImage::image() const {
    return _image;
}
