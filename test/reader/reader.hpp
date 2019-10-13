#pragma once

#include <string_view>

namespace cti {
    class Ticket;
}

namespace cti::reader {
    class TestCase;

    Ticket* getTemplateOf(std::string_view path);
    TestCase* getTestOf(std::string_view path);
}