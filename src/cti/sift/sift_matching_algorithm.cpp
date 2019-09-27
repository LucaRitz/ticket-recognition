#include "sift_matching_algorithm.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket_match.hpp>

void cti::SiftMatchingAlgorithm::train(const cti::Ticket& ticketTemplate) {
    // TODO: implement
}

void cti::SiftMatchingAlgorithm::train(const vector<const cti::Ticket*>& ticketTemplates) {
    // TODO: implement
}

vector<cti::TicketMatch> cti::SiftMatchingAlgorithm::execute(const cti::TicketImage& ticket) const {
    return vector<cti::TicketMatch>();
}

const cti::TicketMatch cti::SiftMatchingAlgorithm::select(vector<cti::TicketMatch>& matches) const {
    return cti::TicketMatch("this_template_matched");
}

