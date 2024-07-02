#include "vulkan_device.h"

#include "core/logger.h"
#include "core/boobs_string.h"
#include "core/boobs_memory.h"
#include "containers/darray.h"

typedef struct vulkan_physical_device_requirements {
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;

    const char** device_extension_names;
    b8 sampler_anistropy;
    b8 discrete_gpu;
} vulkan_physical_device_requirements;

typedef struct vulkan_physical_device_queue_family_info {
    u32 graphics_family_index;
    u32 present_family_index;
    u32 compute_family_index;
    u32 transfer_family_index;
} vulkan_physical_device_queue_family_info;

b8 select_physical_device(vulkan_context* context);
b8 physical_device_meets_requirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const vulkan_physical_device_requirements* requirements,
    vulkan_physical_device_queue_family_info* out_queue_info,
    vulkan_swapchain_support_info* out_swapchain_support
);

b8 vulkan_device_create(vulkan_context* context) {
    if (!select_physical_device(context)) {
        BOOBS_FATAL("failed to select physical device");

        return FALSE;
    }

    BOOBS_INFO("created vulkan physical device")

    b8 present_shares_graphics_queue = context->device.graphics_queue_index == context->device.present_queue_index;
    b8 transfer_shares_graphics_queue = context->device.graphics_queue_index == context->device.transfer_queue_index;
    u32 index_count = 1;

    if (!present_shares_graphics_queue) {
        index_count++;
    }

    if (!transfer_shares_graphics_queue) {
        index_count++;
    }

    u32 indices[index_count];
    u8 index = 0;
    indices[index++] = context->device.graphics_queue_index;

    if (!present_shares_graphics_queue) {
        indices[index++] = context->device.present_queue_index;
    }

    if (!transfer_shares_graphics_queue) {
        indices[index++] = context->device.transfer_queue_index;
    }

    VkDeviceQueueCreateInfo queue_create_infos[index_count];
    for (u32 i = 0; i < index_count; ++i) {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = indices[i];
        queue_create_infos[i].queueCount = 1;

        // if (indices[i] == context->device.graphics_queue_index) {
        //     queue_create_infos[i].queueCount = 2;
        // }

        queue_create_infos[i].flags = 0;
        queue_create_infos[i].pNext = 0;

        f32 queue_priority = 1.0f;
        queue_create_infos[i].pQueuePriorities = &queue_priority;
    }

    VkPhysicalDeviceFeatures device_features = {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    device_create_info.queueCreateInfoCount = index_count;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 1;

    const char* extension_names = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    device_create_info.ppEnabledExtensionNames = &extension_names;

    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = 0;

    VK_CHECK(vkCreateDevice(context->device.physical_device, &device_create_info, context->allocator, &context->device.logical_device));
    BOOBS_INFO("created vulkan logical device");

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.graphics_queue_index,
        0,
        &context->device.graphics_queue
    );

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.present_queue_index,
        0,
        &context->device.present_queue
    );

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.transfer_queue_index,
        0,
        &context->device.transfer_queue
    );

    BOOBS_INFO("got device queues");

    return TRUE;
}

void vulkan_device_destroy(vulkan_context* context) {
    context->device.graphics_queue = 0;
    context->device.present_queue = 0;
    context->device.transfer_queue = 0;

    if (context->device.logical_device) {
        vkDestroyDevice(context->device.logical_device, context->allocator);

        context->device.logical_device = 0;
        BOOBS_INFO("destroyed vulkan logical device");
    }

    context->device.physical_device = 0;
    BOOBS_INFO("destroyed vulkan physical device");

    if (context->device.swapchain_support.formats) {
        boobs_free(
            context->device.swapchain_support.formats,
            sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support.format_count,
            MEMORY_TAG_RENDERER
        );

        context->device.swapchain_support.formats = 0;
        context->device.swapchain_support.format_count = 0;
    }

    BOOBS_INFO("freed formats");

    if (context->device.swapchain_support.present_modes) {
        boobs_free(
            context->device.swapchain_support.present_modes,
            sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support.present_mode_count,
            MEMORY_TAG_RENDERER
        );

        context->device.swapchain_support.present_modes = 0;
        context->device.swapchain_support.present_mode_count = 0;
    }

    BOOBS_INFO("freed present modes")

    boobs_zero_memory(
        &context->device.swapchain_support.capabilities,
        sizeof(context->device.swapchain_support.capabilities)
    );

    BOOBS_INFO("freed capabilities");

    context->device.graphics_queue_index = -1;
    context->device.present_queue_index = -1;
    context->device.transfer_queue_index = -1;

    BOOBS_INFO("reset queue indexes");
}

void vulkan_device_query_swapchain_support(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface,
    vulkan_swapchain_support_info* out_support_info
) {
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &out_support_info->capabilities));
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &out_support_info->format_count, 0));

    if (out_support_info->format_count != 0) {
        if (!out_support_info->formats) {
            out_support_info->formats = boobs_allocate(
                sizeof(VkSurfaceFormatKHR) * out_support_info->format_count,
                MEMORY_TAG_RENDERER
            );
        }

        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &out_support_info->format_count, out_support_info->formats));
    }

    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &out_support_info->present_mode_count, 0));

    if (out_support_info->present_mode_count != 0) {
        if (!out_support_info->present_modes) {
            out_support_info->present_modes = boobs_allocate(
                sizeof(VkPresentModeKHR) * out_support_info->present_mode_count,
                MEMORY_TAG_RENDERER
            );
        }

        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &out_support_info->present_mode_count, out_support_info->present_modes));
    }
}

b8 select_physical_device(vulkan_context* context) {
    u32 physical_device_count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, 0));

    if (physical_device_count == 0) {
        BOOBS_FATAL("failed to find physical device which supports vulkan");

        return FALSE;
    }

    VkPhysicalDevice physical_devices[physical_device_count];
    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices));

    for (u32 i = 0; i < physical_device_count; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

        vulkan_physical_device_requirements requirements = {};
        requirements.graphics = TRUE;
        requirements.present = TRUE;
        requirements.transfer = TRUE;
        requirements.sampler_anistropy = TRUE;
        requirements.discrete_gpu = TRUE;
        requirements.device_extension_names = darray_create(const char*);

        darray_push(requirements.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        vulkan_physical_device_queue_family_info queue_info = {};
        b8 result = physical_device_meets_requirements(
            physical_devices[i],
            context->surface,
            &properties,
            &features,
            &requirements,
            &queue_info,
            &context->device.swapchain_support
        );

        if (result) {
            BOOBS_INFO("selected device %s", properties.deviceName);
            BOOBS_INFO(
                "GPU driver version %d.%d.%d",
                VK_VERSION_MAJOR(properties.driverVersion),
                VK_VERSION_MINOR(properties.driverVersion),
                VK_VERSION_PATCH(properties.driverVersion)
            )

            BOOBS_INFO(
                "vulkan API version %d.%d.%d",
                VK_VERSION_MAJOR(properties.apiVersion),
                VK_VERSION_MINOR(properties.apiVersion),
                VK_VERSION_PATCH(properties.apiVersion)
            )

            for (u32 j = 0; j < memory.memoryHeapCount; ++j) {
                f32 memory_size_gib = (((f32)memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);

                if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    BOOBS_INFO("local GPU memory: %.2f GiB", memory_size_gib);
                } else {
                    BOOBS_INFO("shared system memory: %.2f GiB", memory_size_gib);
                }
            }

            context->device.physical_device = physical_devices[i];
            context->device.graphics_queue_index = queue_info.graphics_family_index;
            context->device.present_queue_index = queue_info.present_family_index;
            context->device.transfer_queue_index = queue_info.transfer_family_index;

            context->device.properties = properties;
            context->device.features = features;
            context->device.memory = memory;

            break;
        }
    }

    if (!context->device.physical_device) {
        BOOBS_ERROR("no physical devices which meet the requirements were found");

        return FALSE;
    }

    BOOBS_INFO("selected physical device");

    return TRUE;
}

b8 physical_device_meets_requirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const vulkan_physical_device_requirements* requirements,
    vulkan_physical_device_queue_family_info* out_queue_info,
    vulkan_swapchain_support_info* out_swapchain_support
) {
    out_queue_info->graphics_family_index = -1;
    out_queue_info->present_family_index = -1;
    out_queue_info->compute_family_index = -1;
    out_queue_info->transfer_family_index = -1;

    if (requirements->discrete_gpu) {
        if (properties->deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            BOOBS_INFO("current device is not a discrete GPU. skipping");

            return FALSE;
        }
    }

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, 0);
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    BOOBS_INFO("graphics | present | compute | transfer | name");

    u8 min_transfer_score = 255;
    for (u32 i = 0; i < queue_family_count; ++i) {
        u8 current_transfer_score = 0;

        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            out_queue_info->graphics_family_index = i;

            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            out_queue_info->compute_family_index = i;

            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            if (current_transfer_score <= min_transfer_score) {
                min_transfer_score = current_transfer_score;

                out_queue_info->transfer_family_index = i;
            }
        }

        VkBool32 supports_present = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));

        if (supports_present) {
            out_queue_info->present_family_index = i;
        }
    }

    BOOBS_INFO(
        "       %d |       %d |       %d |        %d | %s",
        out_queue_info->graphics_family_index != -1,
        out_queue_info->present_family_index != -1,
        out_queue_info->compute_family_index != -1,
        out_queue_info->transfer_family_index != -1,
        properties->deviceName
    );

    if (
        (!requirements->graphics || (requirements->graphics && out_queue_info->graphics_family_index != -1)) &&
        (!requirements->present  || (requirements->present  && out_queue_info->present_family_index  != -1)) &&
        (!requirements->compute  || (requirements->compute  && out_queue_info->compute_family_index  != -1)) &&
        (!requirements->transfer || (requirements->transfer && out_queue_info->transfer_family_index != -1))
    ) {
        BOOBS_INFO("device meets queue requirements");
        BOOBS_TRACE(
            "GFI=%i PFI=%i CFI=%i TFI=%i",
            out_queue_info->graphics_family_index,
            out_queue_info->present_family_index,
            out_queue_info->compute_family_index,
            out_queue_info->transfer_family_index
        );

        vulkan_device_query_swapchain_support(device, surface, out_swapchain_support);

        if (out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1) {
            if (out_swapchain_support->formats) {
                boobs_free(
                    out_swapchain_support->formats,
                    sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count,
                    MEMORY_TAG_RENDERER
                );
            }

            if (out_swapchain_support->present_modes) {
                boobs_free(
                    out_swapchain_support->present_modes,
                    sizeof(VkPresentModeKHR) * out_swapchain_support->present_mode_count,
                    MEMORY_TAG_RENDERER
                );
            }

            BOOBS_INFO("required swapchain support not present. skipping");

            return FALSE;
        }

        if (requirements->device_extension_names) {
            u32 available_extension_count = 0;
            VkExtensionProperties* available_extensions = 0;

            VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, 0));

            if (available_extension_count != 0) {
                available_extensions = boobs_allocate(
                    sizeof(VkExtensionProperties) * available_extension_count,
                    MEMORY_TAG_RENDERER
                );

                VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, available_extensions));

                u32 required_extension_count = darray_length(requirements->device_extension_names);
                for (u32 i = 0; i < required_extension_count; ++i) {
                    b8 found = FALSE;
                    for (u32 j = 0; j < available_extension_count; ++j) {
                        if (strings_equal(requirements->device_extension_names[i], available_extensions[j].extensionName)) {
                            found = TRUE;

                            break;
                        }
                    }

                    if (!found) {
                        BOOBS_INFO("required extension %s not found. skipping", requirements->device_extension_names[i]);
                        boobs_free(
                            available_extensions,
                            sizeof(VkExtensionProperties) * available_extension_count,
                            MEMORY_TAG_RENDERER
                        );

                        return FALSE;
                    }
                }
            }

            boobs_free(available_extensions, sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TAG_RENDERER);
        }

        if (requirements->sampler_anistropy && !features->samplerAnisotropy) {
            BOOBS_INFO("device does not support sampler anistropy. skipping");

            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}
