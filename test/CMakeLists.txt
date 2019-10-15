project(ticket_recognition_tests)

# Setup testing
set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_subdirectory(lib/googletest-release-1.8.1)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(${ticket_recognition_SOURCE_DIR}/src/cti)
include_directories(${ticket_recognition_SOURCE_DIR}/test)

add_executable(ticket_recognition_test)
add_subdirectory(cti)
add_subdirectory(reader)

target_link_libraries(ticket_recognition_test gtest gtest_main)
target_link_libraries(ticket_recognition_test ticket_recognition_lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})