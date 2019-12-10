target_sources(ticket_recognition_lib PUBLIC
        ocr_algorithms.cpp
        tesseract_ocr_algorithm.hpp
        tesseract_ocr_algorithm.cpp

        ${ticket_recognition_SOURCE_DIR}/src/include/ocr/ocr_algorithm.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/ocr/ocr_algorithms.hpp
        )