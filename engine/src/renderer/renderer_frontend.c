#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "core/logger.h"
#include "core/boobs_memory.h"

static renderer_backend* backend = 0;

b8 renderer_initialize(const char* application_name, struct platform_state* plat_state) {
    backend = boobs_allocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);

    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if (!backend->initialize(backend, application_name, plat_state)) {
        BOOBS_FATAL("failed to initialize renderer backend");

        return FALSE;
    }

    BOOBS_INFO("renderer frontend initialized");

    return TRUE;
}

void renderer_shutdown() {
    backend->shutdown(backend);
    boobs_free(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}

b8 renderer_begin_frame(f32 dt) {
    return backend->begin_frame(backend, dt);
}

b8 renderer_end_frame(f32 dt) {
    b8 result = backend->end_frame(backend, dt);
    backend->frame_number++;

    return result;
}

b8 renderer_draw_frame(render_packet* packet) {
    if (renderer_begin_frame(packet->dt)) {
        b8 result = renderer_end_frame(packet->dt);

        if (!result) {
            BOOBS_ERROR("renderer_end_frame() failed. cannot recover");

            return FALSE;
        }
    }

    return TRUE;
}