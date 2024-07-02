#include "vulkan_backend.h"
#include "vulkan_types.inl"
#include "vulkan_platform.h"

#include "core/logger.h"
#include "core/boobs_string.h"

#include "containers/darray.h"

#include "platform/platform.h"

static vulkan_context context;

b8 vulkan_renderer_backend_initialize(renderer_backend* backend, const char* application_name, struct platform_state* plat_state) {
    // todo: custom allocator, use driver defaults atm
    context.allocator = 0;

    VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Boobs";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    create_info.pApplicationInfo = &app_info;

    const char** required_extensions = darray_create(const char*);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);       // generic surface extension
    platform_get_required_extension_names(&required_extensions);            // platform-specific extensions

#   if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);   // debug utilities
    BOOBS_DEBUG("required extensions:")

    u32 length = darray_length(required_extensions);
    for (u32 i = 0; i < length; ++i) {
        BOOBS_DEBUG(required_extensions[i]);
    }
#   endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    const char** required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

#   if defined(_DEBUG)
    BOOBS_INFO("enumerating validation layers");

    required_validation_layer_names = darray_create(const char*);
    darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layer_names);

    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    for (u32 i = 0; i < required_validation_layer_count; ++i) {
        BOOBS_INFO("searching for layer %s", required_validation_layer_names[i]);

        b8 found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if (strings_equal(required_validation_layer_names[i], available_layers[j].layerName)) {
                found = TRUE;

                BOOBS_INFO("found layer %s", required_validation_layer_names[i]);

                break;
            }
        }

        if (!found) {
            BOOBS_FATAL("missing validation layer %s", required_validation_layer_names[i]);

            return FALSE;
        }
    }

    BOOBS_INFO("all validation layers present");
#   endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance))
    BOOBS_INFO("vulkan renderer backend initialized");

    return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend* backend) {

}

void vulkan_renderer_backend_on_resized(renderer_backend* backend, u16 width, u16 height) {

}

b8 vulkan_renderer_backend_begin_frame(renderer_backend* backend, f32 dt) {
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(renderer_backend* backend, f32 dt) {
    return TRUE;
}
