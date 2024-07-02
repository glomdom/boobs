cmake_minimum_required(VERSION 3.10)
project(testbed)

set(assembly "testbed")
file(GLOB_RECURSE cfiles CONFIGURE_DEPENDS "*.c")

include_directories(src ../engine/src)
add_executable(${assembly} ${cfiles})

if(WIN32)
    target_compile_options(${assembly} PRIVATE -g -D_DEBUG -DBOOBS_IMPORT)
    target_include_directories(${assembly} PRIVATE ../engine/src)
    target_link_libraries(${assembly} PRIVATE boobs)
else()
    target_compile_options(${assembly} PRIVATE -g -fdeclspec -fPIC -D_DEBUG -DBOOBS_IMPORT)
    target_include_directories(${assembly} PRIVATE ../engine/src)
    target_link_libraries(${assembly} PRIVATE boobs)
    target_link_directories(${assembly} PRIVATE ${CMAKE_BINARY_DIR}/bin)
endif()

install(TARGETS ${assembly}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)