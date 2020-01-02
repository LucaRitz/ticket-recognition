cmake_minimum_required(VERSION 3.14)
project(ticket_recognition)

if (LINUX_BUILD)
    set(BUILD_ENV "linux")
    set(LIB_TYPE "so")
else(LINUX_BUID)
    set(BUILD_ENV "windows")
    set(LIB_TYPE "dll")
endif (LINUX_BUILD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

option(CTI_DEBUG_TOGGLE "CTI Debugging" ON)

if(CTI_DEBUG_TOGGLE)
    message("CTI_DEBUG_TOGGLE is ON")
    add_definitions(-DCTI_DEBUG)
else()
    message("CTI_DEBUG_TOGGLE is OFF")
endif()

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

add_subdirectory(src)

target_link_libraries(ticket_recognition_lib ${OpenCV_LIBS})
target_link_libraries(ticket_recognition_lib ${Leptonica_LIBRARIES})
target_link_libraries(ticket_recognition_lib ${Tesseract_LIBRARIES})

add_subdirectory(test)

# Copy all libraries to binary folder
set(LIBRARY_BINARY_DIR ${PROJECT_BINARY_DIR}/libs)
file(MAKE_DIRECTORY ${LIBRARY_BINARY_DIR})

if (LINUX_BUILD)
    set(OPENCV_LIB_ENV "${BUILD_ENV}/lib")
else(LINUX_BUID)
    set(OPENCV_LIB_ENV "${BUILD_ENV}/x64/vc15/bin")
endif (LINUX_BUILD)

file(GLOB OPENCV_ALL_LIBS "${OpenCV_DIR}/${OPENCV_LIB_ENV}/*.${LIB_TYPE}*")
file(COPY ${OPENCV_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB LPNG_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/lpng/${BUILD_ENV}/bin/*.${LIB_TYPE}*")
file(COPY ${LPNG_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB TIFF_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/tiff/${BUILD_ENV}/bin/*.${LIB_TYPE}*")
file(COPY ${TIFF_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB ZLIB_ALL_LIBS "${ticket_recognition_SOURCE_DIR}/libs/zlib/${BUILD_ENV}/bin/*.${LIB_TYPE}*")
file(COPY ${ZLIB_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB LEPTONICA_ALL_LIBS "${Leptonica_DIR}/${BUILD_ENV}/bin/*.${LIB_TYPE}*")
file(COPY ${LEPTONICA_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})

file(GLOB TESSERACT_ALL_LIBS "${Tesseract_DIR}/${BUILD_ENV}/bin/*.${LIB_TYPE}*")
file(COPY ${TESSERACT_ALL_LIBS} DESTINATION ${LIBRARY_BINARY_DIR})