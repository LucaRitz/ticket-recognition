#include <include/text.hpp>

cti::Text::Text(string key, cti::BoundingBox &boundingBox) : _key(std::move(key)), _boundingBox(boundingBox) {

}

const string cti::Text::key() const {
    return _key;
}

const cti::BoundingBox& cti::Text::boundingBox() const {
    return _boundingBox;
}