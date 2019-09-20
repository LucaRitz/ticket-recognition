cmake_minimum_required(VERSION 3.14)
project(ticket_recognition)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(docs/doxygen.cmake)

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

add_library(ticket_recognition_lib SHARED)
target_compile_definitions(ticket_recognition_lib PUBLIC MAKEDLL)
add_executable(ticket_recognition)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(ticket_recognition_lib ${OpenCV_LIBS})
target_link_libraries(ticket_recognition_lib ${Leptonica_LIBRARIES})
target_link_libraries(ticket_recognition_lib ${Tesseract_LIBRARIES})
target_link_libraries(ticket_recognition ticket_recognition_lib)

# Copy all libraries to binary folder
set(LIBRARY_BINARY_DIR ${PROJECT_BINARY_DIR}/libs)
file(MAKE_DIRECTORY ${LIBRARY_BINARY_DIR})

file(GLOB OPENCV_ALL_LIBS "${OpenCV_DIR}/x64/vc15/bin/*.dll")
file(COPY ${OPENCV_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB LPNG_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/lpng/bin/*.dll")
file(COPY ${LPNG_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB TIFF_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/tiff/bin/*.dll")
file(COPY ${TIFF_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB ZLIB_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/zlib/bin/*.dll")
file(COPY ${ZLIB_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB LEPTONICA_ALL_LIBS "${Leptonica_DIR}/bin/*.dll")
file(COPY ${LEPTONICA_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB TESSERACT_ALL_LIBS "${Tesseract_DIR}/bin/*.dll")
file(COPY ${TESSERACT_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})