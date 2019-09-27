#pragma once

#include <include/algorithm/extraction_algorithm.hpp>

namespace cti {

    class SiftExtractionAlgorithm : public ExtractionAlgorithm {
    public:
        void normalize(const Ticket&, TicketImage&) const override;
        Metadata* read(const Ticket&, TicketImage&) const override;
    };
}