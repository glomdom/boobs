add_executable(testbed
    src/entry.c
    src/game.c
)

target_link_libraries(testbed PRIVATE engine)

set_target_properties(testbed PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(testbed PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)