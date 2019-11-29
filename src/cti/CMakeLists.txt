target_sources(ticket_recognition_lib PUBLIC
        matcher.cpp
        metadata_reader.cpp
        ticket_image.cpp
        point.cpp
        bounding_box.cpp
        text.cpp
        metadata.cpp
        ticket_match.cpp
        ticket.cpp
        )
add_subdirectory(matching)
add_subdirectory(extraction)
add_subdirectory(ocr)
add_subdirectory(timer)
