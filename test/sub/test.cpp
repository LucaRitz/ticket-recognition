#include <gtest/gtest.h>
#include "some/someClass.hpp"

TEST(first_test, testdescription) {
    // Act
    int b = test(5);

    // Assert
    ASSERT_EQ(1, b);
}

TEST(second, test) {
    char* str = "resources\\test.jpg";
    display(str);
}