#include <include/metadata_reader.hpp>
#include <include/algorithm/extraction_algorithm.hpp>
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/metadata.hpp>

cti::MetadataReader::MetadataReader(const cti::ExtractionAlgorithm& algorithm) : _algorithm(algorithm) {}

const cti::Metadata* const cti::MetadataReader::read(const cti::Ticket& ticketTemplate, cti::TicketImage& ticket) {
    _algorithm.normalize(ticketTemplate, ticket);
    return _algorithm.read(ticketTemplate, ticket);
}