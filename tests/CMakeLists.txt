add_executable(tests
    src/main.c
    src/test_manager.c
    src/memory/linear_allocator_tests.c
)

target_link_libraries(tests PRIVATE engine)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)