#include "vulkan_fence.h"

#include "core/logger.h"

void vulkan_fence_create(vulkan_context* context, b8 create_signaled, vulkan_fence* out_fence) {
    out_fence->is_signaled = create_signaled;

    VkFenceCreateInfo fence_create_info = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    if (out_fence->is_signaled) {
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VK_CHECK(vkCreateFence(context->device.logical_device, &fence_create_info, context->allocator, &out_fence->handle));
}

void vulkan_fence_destroy(vulkan_context* context, vulkan_fence* fence) {
    if (fence->handle) {
        vkDestroyFence(context->device.logical_device, fence->handle, context->allocator);

        fence->handle = 0;
    }

    fence->is_signaled = FALSE;

    BOOBS_INFO("destroyed fence");
}

b8 vulkan_fence_wait(vulkan_context* context, vulkan_fence* fence, u64 timeout_ns) {
    if (!fence->is_signaled) {
        VkResult result = vkWaitForFences(context->device.logical_device, 1, &fence->handle, TRUE, timeout_ns);

        switch (result) {
            case VK_SUCCESS: {
                fence->is_signaled = TRUE;

                return TRUE;
            } break;

            case VK_TIMEOUT: {
                BOOBS_WARN("vk_fence_wait() timed out");
            } break;

            case VK_ERROR_DEVICE_LOST: {
                BOOBS_ERROR("vk_fence_wait() lost device");
            } break;

            case VK_ERROR_OUT_OF_HOST_MEMORY: {
                BOOBS_ERROR("vk_fence_wait() host out of memory");
            } break;

            case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
                BOOBS_ERROR("vk_fence_wait() device out of memory");
            } break;

            default: {
                BOOBS_ERROR("vk_fence_wait() unknown error");
            } break;
        }
    } else {
        return TRUE;
    }

    return FALSE;
}

void vulkan_fence_reset(vulkan_context* context, vulkan_fence* fence) {
    if (fence->is_signaled) {
        VK_CHECK(vkResetFences(context->device.logical_device, 1, &fence->handle));

        fence->is_signaled = FALSE;
    }
}
