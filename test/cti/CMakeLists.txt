target_sources(ticket_recognition_test PRIVATE
        matching_algorithm_test.cpp
        extraction_algorithm_test.cpp
        boundingbox_test.cpp
        ticket_test.cpp
        ticket_image_test.cpp
        test_util.hpp
        test_util.cpp)

add_subdirectory(matching)