#pragma once

#include <string_view>
#include <vector>

using std::vector;

namespace cti {
    class Ticket;
}

namespace cti::reader {
    class TestCase;

    vector<const Ticket*> getAllTemplatesOf(std::string_view);
    vector<TestCase*> getAllTestsOf(std::string_view);
}