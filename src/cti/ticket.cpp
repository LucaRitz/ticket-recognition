#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>

cti::Ticket::Ticket(string name, const cti::TicketImage &image, vector<const cti::Text *> &texts)
        : Ticket(std::move(name), image, texts, *(new vector<const BoundingBox*>())) {

}

cti::Ticket::Ticket(string name, const cti::TicketImage &image, vector<const cti::Text *> &texts,
                    vector<const cti::BoundingBox *> &matchingAreas)
        : _name(std::move(name)), _image(image), _texts(texts), _matchingAreas(matchingAreas) {

}

const string cti::Ticket::name() {
    return _name;
}

const cti::TicketImage& cti::Ticket::image() const {
    return _image;
}

const vector<const cti::Text*>& cti::Ticket::texts() const {
    return _texts;
}

const vector<const cti::BoundingBox*>& cti::Ticket::matchingAreas() const {
    return _matchingAreas;
}