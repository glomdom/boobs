#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "core/logger.h"

typedef struct renderer_system_state {
    renderer_backend backend;
} renderer_system_state;

static renderer_system_state* state_ptr;

b8 renderer_system_initialize(u64* memory_requirement, void* state, const char* application_name) {
    *memory_requirement = sizeof(renderer_system_state);
    if (state == 0) {
        return true;
    }

    state_ptr = state;

    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, &state_ptr->backend);
    state_ptr->backend.frame_number = 0;

    if (!state_ptr->backend.initialize(&state_ptr->backend, application_name)) {
        BOOBS_FATAL("failed to initialize renderer backend");

        return false;
    }

    BOOBS_INFO("renderer frontend initialized");

    return true;
}

void renderer_system_shutdown(void* state) {
    if (state_ptr) {
        state_ptr->backend.shutdown(&state_ptr->backend);
    }

    state_ptr = 0;

    BOOBS_INFO("renderer system has shutdown");
}

b8 renderer_begin_frame(f32 dt) {
    if (!state_ptr) {
        return false;
    }

    return state_ptr->backend.begin_frame(&state_ptr->backend, dt);
}

b8 renderer_end_frame(f32 dt) {
    if (!state_ptr) {
        return false;
    }

    b8 result = state_ptr->backend.end_frame(&state_ptr->backend, dt);
    state_ptr->backend.frame_number++;

    return result;
}


void renderer_on_resize(u16 width, u16 height) {
    if (state_ptr) {
        state_ptr->backend.resized(&state_ptr->backend, width, height);
    } else {
        BOOBS_WARN("renderer backend does not exist");
    }
}

b8 renderer_draw_frame(render_packet* packet) {
    if (renderer_begin_frame(packet->dt)) {
        b8 result = renderer_end_frame(packet->dt);

        if (!result) {
            BOOBS_ERROR("renderer_end_frame() failed. cannot recover");

            return false;
        }
    }

    return true;
}
