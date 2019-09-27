target_sources(ticket_recognition_lib PUBLIC
        bounding_box.hpp
        matcher.hpp
        metadata.hpp
        metadata_reader.hpp
        point.hpp
        text.hpp
        ticket.hpp
        ticket_image.hpp
        ticket_match.hpp)
add_subdirectory(algorithm)
add_subdirectory(sift)