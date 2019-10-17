#pragma once

#include <string_view>
#include <vector>

using std::vector;

namespace cti {
    class Ticket;
}

namespace cti::reader {
    class TestCase;

    vector<Ticket*> getAllTemplatesOf(std::string_view);
    Ticket* getTemplateOf(std::string_view);
    TestCase* getTestOf(std::string_view);
}