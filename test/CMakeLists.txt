project(ticket_recognition_tests)


# Setup testing
enable_testing()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(GTEST_ROOT "${ticket_recognition_SOURCE_DIR}/libs/googletest-distribution")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(TICKET_RECOGNITION_TEST_SOURCES)
add_subdirectory(sub)
add_executable(ticket_recognition_test ${TICKET_RECOGNITION_TEST_SOURCES})

target_link_libraries(ticket_recognition_test GTest::gtest GTest::gtest_main)
target_link_libraries(ticket_recognition_test ticket_recognition_lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})