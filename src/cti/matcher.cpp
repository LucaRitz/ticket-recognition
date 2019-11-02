#include <include/matcher.hpp>
#include <include/algorithm/matching_algorithm.hpp>
#include <include/ticket.hpp>
#include <include/ticket_match.hpp>
#include <include/ticket_image.hpp>

cti::Matcher::Matcher(cti::MatchingAlgorithm &algorithm) : _algorithm(algorithm) {}

void cti::Matcher::train(const cti::Ticket& ticketTemplate) {
    _algorithm.train(ticketTemplate);
}

void cti::Matcher::train(const vector<const cti::Ticket*>& ticketTemplates) {
    _algorithm.train(ticketTemplates);
}

const std::optional<const cti::TicketMatch> cti::Matcher::match(const cti::TicketImage& ticket) const {
    vector<TicketMatch> matches = _algorithm.execute(ticket);
    const TicketMatch match = _algorithm.select(matches);
    return std::make_optional(match);
}