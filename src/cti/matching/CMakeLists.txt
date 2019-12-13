target_sources(ticket_recognition_lib PUBLIC
        matching_algorithm_impl.hpp
        matching_algorithm_impl.cpp
        matching_algorithms.cpp

        ${ticket_recognition_SOURCE_DIR}/src/include/matching/matching_algorithm.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/matching/matching_algorithms.hpp
        )