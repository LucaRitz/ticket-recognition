#include <include/point.hpp>

cti::Point::Point(int x, int y) : _x(x), _y(y) {

}

const int cti::Point::x() const {
    return _x;
}

const int cti::Point::y() const {
    return _y;
}