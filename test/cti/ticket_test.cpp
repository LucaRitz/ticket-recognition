#include <gtest/gtest.h>
#include <include/exception/cti_exception.hpp>
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <include/text.hpp>
#include "../io/file_util.hpp"
#include <vector>

using cti::CtiException;
using cti::Ticket;
using cti::TicketImage;
using cti::BoundingBox;
using cti::Point;
using cti::Text;
using std::vector;

TEST(ticket, noexception) {
    TicketImage image { "resources/images/boulderisland.jpg" };
    vector<const Text*> texts;
    BoundingBox box {Point{40, 45}, Point{105, 140}};
    const Text text {"code", box};
    texts.push_back(&text);

    Ticket ticket { "name", image, texts };
}

TEST(ticket, text_boundingbox_outside_image_bounds) {
    TicketImage image { "resources/images/boulderisland.jpg" };
    vector<const Text*> texts;
    BoundingBox box {Point{751, 300}, Point{760, 310}};
    const Text text {"code", box};
    texts.push_back(&text);
    const std::string expectedMessage("Text BoundingBox must be withing TicketImage bounds");

    // Act
    try {
        Ticket ticket { "name", image, texts };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}
