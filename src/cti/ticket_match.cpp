#include <include/ticket_match.hpp>

#include <include/ticket.hpp>

cti::TicketMatch::TicketMatch(const cti::Ticket& ticket) : _ticket(ticket) {

}

cti::TicketMatch::TicketMatch(const cti::TicketMatch& other) : _ticket(other._ticket) {

}

cti::TicketMatch::TicketMatch(cti::TicketMatch&& other) : _ticket(other._ticket) {

}

const cti::Ticket& cti::TicketMatch::ticket() const {
    return _ticket;
}