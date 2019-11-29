#include <include/metadata_reader.hpp>
#include <include/extraction/extraction_algorithm.hpp>
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/metadata.hpp>

cti::MetadataReader::MetadataReader(const cti::ExtractionAlgorithm& algorithm) : _algorithm(algorithm) {}

const cti::Metadata* const cti::MetadataReader::read(const cti::Ticket& ticketTemplate, const cti::TicketImage& ticket) {
    cti::TicketImage* normalizedImage = _algorithm.normalize(ticketTemplate, ticket);
    if(normalizedImage == nullptr) {
        std::unordered_map<string, string> map;
        return new cti::Metadata(map);
    }
    return _algorithm.read(ticketTemplate, *normalizedImage);
}