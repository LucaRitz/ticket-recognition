#include <include/ticket.hpp>
#include "./test_util.hpp"

using cti::Ticket;

const cti::Ticket* cti::reader::findTicket(std::vector<const Ticket*>& tickets, const std::string& name) {
    for (auto* ticket : tickets) {
        if (ticket->name() == name) {
            return ticket;
        }
    }
    return nullptr;
}
