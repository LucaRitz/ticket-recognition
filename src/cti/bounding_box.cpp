#include <include/bounding_box.hpp>
#include <include/exception/cti_exception.hpp>

int isValid(const cti::Point& topLeft, const cti::Point& bottomRight) {

    return (bottomRight.x() - topLeft.x()) > 0
        && (bottomRight.y() - topLeft.y()) > 0;
}

cti::BoundingBox::BoundingBox(const cti::Point topLeft, const cti::Point bottomRight)
    : _topLeft(topLeft), _bottomRight(bottomRight) {

    if(!isValid(topLeft, bottomRight)) {
        throw CtiException { "BoundingBox Points span a rectangle with a negative area." };
    }
}

const cti::Point& cti::BoundingBox::topLeft() const {
    return _topLeft;
}

const cti::Point& cti::BoundingBox::bottomRight() const {
    return _bottomRight;
}
