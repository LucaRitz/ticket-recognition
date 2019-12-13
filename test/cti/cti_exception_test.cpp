#include <gtest/gtest.h>
#include <include/exception/cti_exception.hpp>

using cti::CtiException;

namespace cti::test {
    void thisFunctionThrowsACtiException(const std::string&) noexcept(false);
}

TEST(cti_exception, throwAndCatchCorrectly) {
    const std::string expectedMessage("Expected message");

    // Act
    try {
        cti::test::thisFunctionThrowsACtiException(expectedMessage);
    } catch (const CtiException& exc) {
        ASSERT_STREQ(expectedMessage.c_str(), exc.what());
    }
}

namespace cti::test {
    void thisFunctionThrowsACtiException(const std::string& message) noexcept(false) {
        throw CtiException(message);
    }
}