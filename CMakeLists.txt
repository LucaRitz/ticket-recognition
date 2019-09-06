cmake_minimum_required(VERSION 3.14)
project(ticket_recognition)

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "${ticket_recognition_SOURCE_DIR}/libs/opencv")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${ticket_recognition_SOURCE_DIR}/src)

add_library(ticket_recognition_lib SHARED)
add_executable(ticket_recognition)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(ticket_recognition_lib ${OpenCV_LIBS})
target_link_libraries(ticket_recognition ticket_recognition_lib)