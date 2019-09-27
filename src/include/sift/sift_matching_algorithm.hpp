#pragma once

#include <include/algorithm/matching_algorithm.hpp>

namespace cti {

    class SiftMatchingAlgorithm : public MatchingAlgorithm {
    public:
        void train(const Ticket&) override;
        void train(const vector<const Ticket*>&) override;
        vector<TicketMatch> execute(const TicketImage&) const override;
        const TicketMatch select(vector<TicketMatch>&) const override;
    };
}