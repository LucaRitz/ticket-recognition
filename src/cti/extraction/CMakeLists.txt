target_sources(ticket_recognition_lib PUBLIC
        extraction_algorithm_impl.hpp
        extraction_algorithm_impl.cpp
        extraction_algorithms.cpp
        extraction_options.cpp

        ${ticket_recognition_SOURCE_DIR}/src/include/extraction/extraction_algorithm.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/extraction/extraction_algorithms.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/extraction/extraction_options.hpp
        )