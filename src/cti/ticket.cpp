#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>

cti::Ticket::Ticket(string name, const cti::TicketImage &image, vector<const cti::Text *> &texts)
        : _name(std::move(name)), _image(image), _texts(texts) {

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
