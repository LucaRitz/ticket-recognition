#include <include/bounding_box.hpp>

cti::BoundingBox::BoundingBox(cti::Point &topLeft, cti::Point &bottomRight)  : _topLeft(topLeft), _bottomRight(bottomRight) {

}

const cti::Point& cti::BoundingBox::topLeft() const {
    return _topLeft;
}

const cti::Point& cti::BoundingBox::bottomRight() const {
    return _bottomRight;
}