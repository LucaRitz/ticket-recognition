#include "sift_extraction_algorithm.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/metadata.hpp>

void cti::SiftExtractionAlgorithm::normalize(const cti::Ticket& ticketTemplate, cti::TicketImage& ticketImage) const {
    // TODO: Implement
}

cti::Metadata *
cti::SiftExtractionAlgorithm::read(const cti::Ticket &ticketTemplate, cti::TicketImage &ticketImage) const {
    std::unordered_map<string, string> map;
    map.insert({"TourCode", "100"});
    return new cti::Metadata(map);
}