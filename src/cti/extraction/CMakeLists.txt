target_sources(ticket_recognition_lib PUBLIC
        extraction_algorithm_impl.hpp
        extraction_algorithm_impl.cpp
        extraction_algorithms.cpp

        ${ticket_recognition_SOURCE_DIR}/src/include/extraction/extraction_algorithm.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/extraction/extraction_algorithms.hpp
        )