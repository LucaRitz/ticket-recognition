#include <gtest/gtest.h>
#include <include/exception/cti_exception.hpp>
#include <include/bounding_box.hpp>

using cti::CtiException;
using cti::BoundingBox;
using cti::Point;

TEST(boundingbox, noexception) {
    BoundingBox boundingBox { Point{ 10,5 }, Point{ 15,10 } };
}

TEST(boundingbox, invalid_x_coordinate) {
    const std::string expectedMessage("BoundingBox Points span a rectangle with a negative area.");

    // Act
    try {
        BoundingBox boundingBox { Point{ 10,5 }, Point{ 10,10 } };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(boundingbox, invalid_y_coordinate) {
    const std::string expectedMessage("BoundingBox Points span a rectangle with a negative area.");

    // Act
    try {
        BoundingBox boundingBox { Point{ 10,5 }, Point{ 15,5 } };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

TEST(boundingbox, invalid_x_and_y_coordinate) {
    const std::string expectedMessage("BoundingBox Points span a rectangle with a negative area.");

    // Act
    try {
        BoundingBox boundingBox { Point{ 10,15 }, Point{ 5,10 } };
        FAIL();
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}
