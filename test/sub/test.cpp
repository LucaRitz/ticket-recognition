#include <include/ticket.hpp>
#include <gtest/gtest.h>

TEST(first_test, testdescription) {
    // Act
    using cti::Cti;
    Cti cti;
    cti.hello("D:\\work\\ticket-recognition\\cmake-build-debug\\resources\\images\\test.png");
    int b = 1;

    // Assert
    ASSERT_EQ(1, b);
}

TEST(second, test) {
    char* str = "resources\\test.jpg";

}