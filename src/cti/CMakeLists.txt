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

        ${ticket_recognition_SOURCE_DIR}/src/include/bounding_box.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/matcher.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/metadata.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/metadata_reader.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/point.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/text.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/ticket.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/ticket_image.hpp
        ${ticket_recognition_SOURCE_DIR}/src/include/ticket_match.hpp
        )
add_subdirectory(matching)
add_subdirectory(extraction)
add_subdirectory(ocr)
add_subdirectory(timer)
add_subdirectory(exception)
