cmake_minimum_required(VERSION 3.10)
project(boobs)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED ENV{VULKAN_SDK})
    message("-- Found VULKAN_SDK: $ENV{VULKAN_SDK}")
else()
    message(FATAL_ERROR "VULKAN_SDK environment variable not set")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

add_subdirectory(engine)
add_subdirectory(testbed)
add_subdirectory(tests)