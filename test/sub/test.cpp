#include <include/ticket.hpp>
#include <gtest/gtest.h>

TEST(first_test, testdescription) {
    // Act
    using cti::Cti;
    Cti cti;
    cti.hello("resources\\tickets\\berner-schlager-open-air-tickets-2019-v3.jpg");
    int b = 1;

    // Assert
    ASSERT_EQ(1, b);
}

TEST(second, test) {
    char* str = "resources\\test.jpg";

}