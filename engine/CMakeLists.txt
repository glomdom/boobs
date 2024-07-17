add_library(engine SHARED
    src/containers/darray.c
    src/core/application.c
    src/core/boobs_memory.c
    src/core/boobs_string.c
    src/core/clock.c
    src/core/event.c
    src/core/input.c
    src/core/logger.c
    src/math/boobs_math.c
    src/memory/linear_allocator.c
    src/platform/platform_linux.c
    src/platform/platform_win32.c
    src/renderer/renderer_backend.c
    src/renderer/renderer_frontend.c
    src/renderer/vulkan/vulkan_backend.c
    src/renderer/vulkan/vulkan_command_buffer.c
    src/renderer/vulkan/vulkan_device.c
    src/renderer/vulkan/vulkan_fence.c
    src/renderer/vulkan/vulkan_framebuffer.c
    src/renderer/vulkan/vulkan_image.c
    src/renderer/vulkan/vulkan_renderpass.c
    src/renderer/vulkan/vulkan_swapchain.c
    src/renderer/vulkan/vulkan_utils.c
)

set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(engine PUBLIC src)

set(ASSET_SOURCE_DIRECTORY
    ${PROJECT_SOURCE_DIR}/assets
)

set(ASSET_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/assets
)

set(SHADER_SOURCE_DIRECTORY
    ${PROJECT_SOURCE_DIR}/assets/shaders)

set(SHADER_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/assets/shaders)

if(WIN32)
    target_include_directories(engine PRIVATE $ENV{VULKAN_SDK}/Include)
    target_link_libraries(engine PRIVATE $ENV{VULKAN_SDK}/Lib/vulkan-1.lib)

    # add_custom_command(TARGET engine POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIRECTORY}
    #     COMMAND $ENV{VULKAN_SDK}/Bin/glslc.exe -fshader-stage=vert ${SHADER_SOURCE_DIRECTORY}/Builtin.ObjectShader.vert.glsl -o ${SHADER_OUTPUT_DIRECTORY}/Builtin.ObjectShader.vert.spv
    #     COMMAND $ENV{VULKAN_SDK}/Bin/glslc.exe -fshader-stage=frag ${SHADER_SOURCE_DIRECTORY}/Builtin.ObjectShader.frag.glsl -o ${SHADER_OUTPUT_DIRECTORY}/Builtin.ObjectShader.frag.spv
    #     COMMENT "Compile builtin object shader")
    
    # add_custom_command(TARGET engine POST_BUILD
    #     COMMAND xcopy ${ASSET_SOURCE_DIRECTORY} ${ASSET_OUTPUT_DIRECTORY} /h /i /c /k /e /r /y
    #     COMMENT "Copy assets to same directory as engine output")
else()
    target_include_directories(engine PRIVATE $ENV{VULKAN_SDK}/include)
    target_link_libraries(engine PRIVATE $ENV{VULKAN_SDK}/lib/libvulkan.so xcb X11 X11-xcb xkbcommon)

    # add_custom_command(TARGET engine POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIRECTORY}
    #     COMMAND $ENV{VULKAN_SDK}/bin/glslc -fshader-stage=vert ${SHADER_SOURCE_DIRECTORY}/Builtin.ObjectShader.vert.glsl -o ${SHADER_OUTPUT_DIRECTORY}/Builtin.ObjectShader.vert.spv
    #     COMMAND $ENV{VULKAN_SDK}/bin/glslc -fshader-stage=frag ${SHADER_SOURCE_DIRECTORY}/Builtin.ObjectShader.frag.glsl -o ${SHADER_OUTPUT_DIRECTORY}/Builtin.ObjectShader.frag.spv
    #     COMMENT "Compile builtin object shader")

    # add_custom_command(TARGET engine POST_BUILD
    #     COMMAND cp -r ${ASSET_SOURCE_DIRECTORY} ${CMAKE_BINARY_DIR}/bin
    #     COMMENT "Copy assets to same directory as engine output")
endif()

target_compile_options(engine PRIVATE -Werror=vla -fdeclspec)
target_compile_definitions(engine PRIVATE BOOBS_EXPORT _CRT_SECURE_NO_WARNINGS)