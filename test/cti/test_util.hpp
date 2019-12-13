#pragma once

#include <string>
#include <vector>

namespace cti {
    class Ticket;
}

namespace cti::reader {

    const cti::Ticket* findTicket(std::vector<const Ticket*>& tickets, const std::string& name);
}