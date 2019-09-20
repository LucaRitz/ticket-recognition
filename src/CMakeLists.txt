add_subdirectory(main)
add_subdirectory(include)
add_subdirectory(some)
add_subdirectory(ticket)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})