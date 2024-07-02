set(CMAKE_C_COMPILER)

add_library(engine SHARED
    src/containers/darray.c
    src/core/application.c
    src/core/boobs_memory.c
    src/core/boobs_string.c
    src/core/clock.c
    src/core/event.c
    src/core/input.c
    src/core/logger.c
    src/platform/platform_linux.c
    src/platform/platform_win32.c
    src/renderer/renderer_backend.c
    src/renderer/renderer_frontend.c
    src/renderer/vulkan/vulkan_backend.c
    src/renderer/vulkan/vulkan_device.c
    src/renderer/vulkan/vulkan_image.c
    src/renderer/vulkan/vulkan_swapchain.c
)

set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(engine PRIVATE $ENV{VULKAN_SDK}/Lib/vulkan-1.lib)

target_include_directories(engine PUBLIC src)
target_include_directories(engine PRIVATE $ENV{VULKAN_SDK}/Include)

target_compile_definitions(engine PRIVATE BOOBS_EXPORT _CRT_SECURE_NO_WARNINGS)