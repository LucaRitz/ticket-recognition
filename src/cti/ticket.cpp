#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>
#include <include/Point.hpp>
#include <include/exception/cti_exception.hpp>

bool isInsideImageBounds(const cti::Point& point, const cti::TicketImage& image) {

    return point.x() >= 0
           && point.y() >= 0
           && point.x() <= image.width()
           && point.y() <= image.height();
}

bool isInsideImageBounds(const cti::BoundingBox& boundingBox, const cti::TicketImage& image) {

    return isInsideImageBounds(boundingBox.topLeft(), image)
        && isInsideImageBounds(boundingBox.bottomRight(), image);
}

cti::Ticket::Ticket(string name, const cti::TicketImage& image, vector<const cti::Text*> &texts)
        : _name(std::move(name)), _image(image), _texts(texts) {

    for(auto text : texts) {
        if(!isInsideImageBounds(text->boundingBox(), image)) {
            throw CtiException { "Text BoundingBox must be withing TicketImage bounds" };
        }
    }
}

const string cti::Ticket::name() const {
    return _name;
}

const cti::TicketImage& cti::Ticket::image() const {
    return _image;
}

const vector<const cti::Text*>& cti::Ticket::texts() const {
    return _texts;
}
