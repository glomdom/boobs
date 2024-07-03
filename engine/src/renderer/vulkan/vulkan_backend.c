#include "vulkan_types.inl"
#include "vulkan_backend.h"
#include "vulkan_platform.h"
#include "vulkan_device.h"
#include "vulkan_swapchain.h"
#include "vulkan_renderpass.h"

#include "core/logger.h"
#include "core/boobs_string.h"

#include "containers/darray.h"

#include "platform/platform.h"

static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
);

i32 find_memory_index(u32 type_filter, u32 property_flags);

b8 vulkan_renderer_backend_initialize(renderer_backend* backend, const char* application_name, struct platform_state* plat_state) {
    context.find_memory_index = find_memory_index;

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
    BOOBS_INFO("created vulkan instance");

#   if defined(_DEBUG)
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    BOOBS_ASSERT_MSG(func, "failed to create debug messenger");

    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    BOOBS_INFO("created vulkan debugger");
#   endif

    if (!platform_create_vulkan_surface(plat_state, &context)) {
        BOOBS_ERROR("failed to create vulkan surface");

        return FALSE;
    }

    BOOBS_INFO("created vulkan surface");

    if (!vulkan_device_create(&context)) {
        BOOBS_ERROR("failed to create vulkan device");

        return FALSE;
    }

    BOOBS_INFO("created vulkan device");

    vulkan_swapchain_create(
        &context,
        context.framebuffer_width,
        context.framebuffer_height,
        &context.swapchain
    );

    vulkan_renderpass_create(
        &context,
        &context.main_renderpass,
        0, 0, context.framebuffer_width, context.framebuffer_height,
        0.0f, 0.0f, 0.2f, 1.0f,
        1.0f,
        0
    );

    BOOBS_INFO("vulkan renderer backend initialized");

    return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend* backend) {
    vulkan_renderpass_destroy(&context, &context.main_renderpass);
    BOOBS_INFO("destroyed renderpass");

    vulkan_swapchain_destroy(&context, &context.swapchain);

    vulkan_device_destroy(&context);
    BOOBS_INFO("destroyed vulkan device");

    if (context.surface) {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);

        context.surface = 0;
    }

    BOOBS_INFO("destroyed vulkan surface")

    if (context.debug_messenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);

        BOOBS_INFO("destroyed vulkan debugger");
    }

    vkDestroyInstance(context.instance, context.allocator);
    BOOBS_INFO("destroyed vulkan instance");
}

void vulkan_renderer_backend_on_resized(renderer_backend* backend, u16 width, u16 height) {

}

b8 vulkan_renderer_backend_begin_frame(renderer_backend* backend, f32 dt) {
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(renderer_backend* backend, f32 dt) {
    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
) {
    switch (message_severity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            BOOBS_ERROR(callback_data->pMessage);
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            BOOBS_WARN(callback_data->pMessage);
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            BOOBS_INFO(callback_data->pMessage);
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            BOOBS_TRACE(callback_data->pMessage);
        } break;
    }

    return VK_FALSE;
}

i32 find_memory_index(u32 type_filter, u32 property_flags) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physical_device, &memory_properties);

    for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
        if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
            return i;
        }
    }

    BOOBS_WARN("failed to find suitable memory type");

    return -1;
}
