cmake_minimum_required(VERSION 3.14)
project(ticket_recognition)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(OpenCV_DIR "${ticket_recognition_SOURCE_DIR}/libs/opencv")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(Leptonica_DIR "${ticket_recognition_SOURCE_DIR}/libs/leptonica")
find_package(Leptonica REQUIRED)
include_directories(${Leptonica_INCLUDE_DIRS})

set(Tesseract_DIR "${ticket_recognition_SOURCE_DIR}/libs/tesseract")
find_package(Tesseract REQUIRED)
include_directories(${Tesseract_INCLUDE_DIRS})

include_directories(${ticket_recognition_SOURCE_DIR}/src)

add_library(ticket_recognition_lib STATIC)
add_executable(ticket_recognition)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(ticket_recognition_lib ${OpenCV_LIBS})
target_link_libraries(ticket_recognition_lib ${Leptonica_LIBRARIES})
target_link_libraries(ticket_recognition_lib ${Tesseract_LIBRARIES})
target_link_libraries(ticket_recognition ticket_recognition_lib)