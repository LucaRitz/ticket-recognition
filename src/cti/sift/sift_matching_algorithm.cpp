#include <include/sift/sift_matching_algorithm.hpp>
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket_match.hpp>
#include "../../some/someClass.hpp"

void cti::SiftMatchingAlgorithm::train(const cti::Ticket& ticketTemplate) {
    this->_trained.clear();
    this->_trained.push_back(&ticketTemplate);
}

void cti::SiftMatchingAlgorithm::train(const vector<const cti::Ticket*>& ticketTemplates) {
    this->_trained.clear();
    for (auto* ticketTemplate : ticketTemplates) {
        this->_trained.push_back(ticketTemplate);
    }
}

vector<cti::TicketMatch> cti::SiftMatchingAlgorithm::execute(const cti::TicketImage& ticket) const {
    char* str = "resources/images/test.png";
    return vector<cti::TicketMatch>();
}

const optional<cti::TicketMatch> cti::SiftMatchingAlgorithm::select(vector<cti::TicketMatch>& matches) const {
    return std::make_optional(cti::TicketMatch(*this->_trained.at(0)));
    //return std::nullopt;
}

