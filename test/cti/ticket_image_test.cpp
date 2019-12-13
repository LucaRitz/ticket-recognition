#include <gtest/gtest.h>
#include <include/exception/cti_exception.hpp>
#include <include/ticket_image.hpp>
#include "../io/file_util.hpp"

using cti::CtiException;
using cti::TicketImage;

TEST(ticketimage, noexception) {
    TicketImage image { "resources/images/boulderisland.jpg" };
}

TEST(ticketimage, file_does_not_exist) {
    const std::string imagePath = "resources/bla/unknown.jpg";
    const std::string expectedMessage("Unable to load image using filepath '"+ imagePath +"'");

    // Act
    try {
        TicketImage image { imagePath };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, no_image_file) {
    const std::string imagePath = "resources/tickets/boulderisland_1.txt";
    const std::string expectedMessage("Unable to load image using filepath '"+ imagePath +"'");

    // Act
    try {
        TicketImage image { imagePath };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, invalid_width) {
    const std::string expectedMessage("TicketImage width must be greater than 0");
    unsigned char imageData[100];

    // Act
    try {
        TicketImage image { 0, 100, 3, 100, imageData };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, invalid_height) {
    const std::string expectedMessage("TicketImage height must be greater than 0");
    unsigned char imageData[100];

    // Act
    try {
        TicketImage image { 100, 0, 3, 100, imageData };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, invalid_bytes_per_pixel) {
    const std::string expectedMessage("TicketImage bytes_per_pixel must be greater than 0");
    unsigned char imageData[100];

    // Act
    try {
        TicketImage image { 50, 100, 0, 100, imageData };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, invalid_bytes_per_line) {
    const std::string expectedMessage("TicketImage bytes_per_line must be greater than 0");
    unsigned char imageData[100];

    // Act
    try {
        TicketImage image { 50, 100, 3, 0, imageData };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(ticketimage, image_data_null) {
    const std::string expectedMessage("TicketImage image pointer must not be NULL");

    // Act
    try {
        TicketImage image { 50, 100, 3, 100, nullptr };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}
