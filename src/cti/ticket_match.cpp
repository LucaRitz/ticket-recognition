#include <include/ticket_match.hpp>

#include <include/ticket.hpp>

cti::TicketMatch::TicketMatch(const cti::Ticket& ticket, const double score) : _ticket(ticket), _score(score) {

}

cti::TicketMatch::TicketMatch(const cti::TicketMatch& other)
    : _ticket(other._ticket), _score(other._score) {

}

cti::TicketMatch::TicketMatch(cti::TicketMatch&& other)
    : _ticket(other._ticket), _score(other._score) {

}

const cti::Ticket& cti::TicketMatch::ticket() const {
    return _ticket;
}

const double cti::TicketMatch::score() const {
    return _score;
}
