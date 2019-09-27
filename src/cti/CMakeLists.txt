target_sources(ticket_recognition_lib PUBLIC
        matcher.cpp
        metadata_reader.cpp
        ticket_image.cpp
        )
add_subdirectory(sift)