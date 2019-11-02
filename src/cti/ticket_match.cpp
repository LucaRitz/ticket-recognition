#include <include/ticket_match.hpp>

cti::TicketMatch::TicketMatch(string name) : _name(std::move(name)) {

}

cti::TicketMatch::TicketMatch(const cti::TicketMatch& other) : _name(other._name) {

}

cti::TicketMatch::TicketMatch(cti::TicketMatch&& other) : _name(other._name) {

}

const string cti::TicketMatch::name() const {
    return _name;
}