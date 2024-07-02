cmake_minimum_required(VERSION 3.10)
project(boobs)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set")
endif()

add_subdirectory(engine)
add_subdirectory(testbed)