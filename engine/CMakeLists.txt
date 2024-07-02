cmake_minimum_required(VERSION 3.10)
project(engine)

set(assembly "boobs")
file(GLOB_RECURSE cfiles CONFIGURE_DEPENDS "*.c")

include_directories(src $ENV{VULKAN_SDK}/include)
add_library(${assembly} SHARED ${cfiles})

if(WIN32)
    target_compile_options(${assembly} PRIVATE -g -Wvarargs -Wall -D_DEBUG -DBOOBS_EXPORT -D_CRT_SECURE_NO_WARNINGS)
    target_include_directories(${assembly} PRIVATE $ENV{VULKAN_SDK}/Include)
    target_link_libraries(${assembly} PRIVATE user32 vulkan-1)
    target_link_directories(${assembly} PRIVATE $ENV{VULKAN_SDK}/Lib)
else()
    target_compile_options(${assembly} PRIVATE -g -fdeclspec -fPIC -D_DEBUG -DBOOBS_EXPORT)
    target_include_directories(${assembly} PRIVATE $ENV{VULKAN_SDK}/include)
    target_link_libraries(${assembly} PRIVATE vulkan xcb X11 X11-xcb xkbcommon)
    target_link_directories(${assembly} PRIVATE $ENV{VULKAN_SDK}/lib /usr/X11R6/lib)
endif()

set_target_properties(${assembly} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${assembly}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)