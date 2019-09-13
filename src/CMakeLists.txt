add_subdirectory(main)
add_subdirectory(some)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})