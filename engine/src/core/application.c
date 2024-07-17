#include "application.h"
#include "game_types.h"
#include "logger.h"

#include "platform/platform.h"
#include "core/boobs_memory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

#include "memory/linear_allocator.h"

#include "renderer/renderer_frontend.h"

typedef struct application_state {
    game* game_inst;

    b8 is_running;
    b8 is_suspended;
    i16 width;
    i16 height;

    clock clock;
    f64 last_time;

    linear_allocator system_allocator;

    u64 event_system_memory_requirement;
    void* event_system_state;

    u64 memory_system_memory_requirement;
    void* memory_system_state;

    u64 logging_system_memory_requirement;
    void* logging_system_state;

    u64 input_system_memory_requirement;
    void* input_system_state;

    u64 platform_system_memory_requirement;
    void* platform_system_state;

    u64 renderer_system_memory_requirement;
    void* renderer_system_state;
} application_state;

static application_state* app_state;

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_resize(u16 code, void* sender, void* listener_inst, event_context context);

b8 application_create(game* game_inst) {
    if (game_inst->application_state) {
        BOOBS_FATAL("application_create called more than once");

        return false;
    }

    game_inst->application_state = boobs_allocate(sizeof(application_state), MEMORY_TAG_APPLICATION);
    app_state = game_inst->application_state;
    app_state->game_inst = game_inst;
    app_state->is_running = false;
    app_state->is_suspended = false;

    u64 systems_allocator_total_size = 64 * 1024 * 1024;  // 64 mb
    linear_allocator_create(systems_allocator_total_size, 0, &app_state->system_allocator);

    // Events
    event_system_initialize(&app_state->event_system_memory_requirement, 0);
    app_state->event_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->event_system_memory_requirement);
    event_system_initialize(&app_state->event_system_memory_requirement, app_state->event_system_state);

    // Memory
    memory_system_initialize(&app_state->memory_system_memory_requirement, 0);
    app_state->memory_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->memory_system_memory_requirement);
    memory_system_initialize(&app_state->memory_system_memory_requirement, app_state->memory_system_state);

    // Logging
    initialize_logging(&app_state->logging_system_memory_requirement, 0);
    app_state->logging_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->logging_system_memory_requirement);

    if (!initialize_logging(&app_state->logging_system_memory_requirement, app_state->logging_system_state)) {
        BOOBS_ERROR("failed to initialize logging system");

        return false;
    }

    // Input
    input_system_initialize(&app_state->input_system_memory_requirement, 0);
    app_state->input_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->input_system_memory_requirement);
    input_system_initialize(&app_state->input_system_memory_requirement, app_state->input_system_state);

    // Register engine-level events
    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    event_register(EVENT_CODE_RESIZED, 0, application_on_resize);

    // Platform
    platform_system_startup(&app_state->platform_system_memory_requirement, 0, 0, 0, 0, 0, 0);
    app_state->platform_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->platform_system_memory_requirement);

    if (!platform_system_startup(
        &app_state->platform_system_memory_requirement,
        app_state->platform_system_state,
        game_inst->app_config.name,
        game_inst->app_config.start_pos_x, game_inst->app_config.start_pos_y,
        game_inst->app_config.start_width, game_inst->app_config.start_height)
    ) {
        return false;
    }

    // Renderer system
    renderer_system_initialize(&app_state->renderer_system_memory_requirement, 0, 0);
    app_state->renderer_system_state = linear_allocator_allocate(&app_state->system_allocator, app_state->renderer_system_memory_requirement);

    if (!renderer_system_initialize(&app_state->renderer_system_memory_requirement, app_state->renderer_system_state, game_inst->app_config.name)) {
        BOOBS_FATAL("renderer failed to initialize");
        return false;

    }

    if (!app_state->game_inst->initialize(app_state->game_inst)) {
        BOOBS_FATAL("game failed to initialize");

        return false;
    }

    app_state->game_inst->on_resize(app_state->game_inst, app_state->width, app_state->height);

    return true;
}

b8 application_run() {
    app_state->is_running = true;
    clock_start(&app_state->clock);
    clock_update(&app_state->clock);
    app_state->last_time = app_state->clock.elapsed;

    f64 running_time = 0;
    u8 frame_count = 0;
    f64 target_frame_seconds = 1.0f / 60;

    BOOBS_INFO(get_memory_usage_str());

    while (app_state->is_running) {
        if (!platform_pump_messages()) {
            app_state->is_running = false;
        }

        if (!app_state->is_suspended) {
            clock_update(&app_state->clock);
            f64 current_time = app_state->clock.elapsed;
            f64 delta = (current_time - app_state->last_time);
            f64 frame_start_time = platform_get_absolute_time();

            if (!app_state->game_inst->update(app_state->game_inst, (f32)delta)) {
                BOOBS_FATAL("game update failed | shutting down");

                app_state->is_running = false;

                break;
            }

            if (!app_state->game_inst->render(app_state->game_inst, (f32)delta)) {
                BOOBS_FATAL("game render failed | shutting down");

                app_state->is_running = false;

                break;
            }

            // fixme: make this way more robust cuz this is disgusting
            render_packet packet;
            packet.dt = delta;

            renderer_draw_frame(&packet);

            f64 frame_end_time = platform_get_absolute_time();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                u64 remaining_ms = (remaining_seconds * 1000);

                b8 limit_frames = false;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            input_update(delta);

            app_state->last_time = current_time;
        }
    }

    app_state->is_running = false;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    input_system_shutdown(app_state->input_system_state);
    renderer_system_shutdown(app_state->renderer_system_state);
    platform_system_shutdown(app_state->platform_system_state);
    memory_system_shutdown(app_state->memory_system_state);
    event_system_shutdown(app_state->event_system_state);

    return true;
}

void application_get_framebuffer_size(u32* width, u32* height) {
    *width = app_state->width;
    *height = app_state->height;
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            BOOBS_INFO("EVENT_CODE_APPLICATION_QUIT recieved");
            app_state->is_running = false;

            return true;
        }
    }

    return false;
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_ESCAPE) {
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            return true;
        } else {
            BOOBS_DEBUG("%c pressed", key_code);
        }
    } else if (code == EVENT_CODE_KEY_RELEASED) {
        u16 key_code = context.data.u16[0];

        BOOBS_DEBUG("%c released", key_code);
    }

    return false;
}

b8 application_on_resize(u16 code, void* sender, void* listener_inst, event_context context) {
    if (code == EVENT_CODE_RESIZED) {
        u16 width = context.data.u16[0];
        u16 height = context.data.u16[1];

        if (width != app_state->width || height != app_state->height) {
            app_state->width = width;
            app_state->height = height;

            if (width == 0 || height == 0) {
                app_state->is_suspended = true;

                return true;
            } else {
                if (app_state->is_suspended) {
                    app_state->is_suspended = false;
                }

                app_state->game_inst->on_resize(app_state->game_inst, width, height);

                renderer_on_resize(width, height);
            }
        }
    }

    return false;
}
