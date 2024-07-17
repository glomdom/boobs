#include "game.h"

#include <core/logger.h>
#include <core/boobs_memory.h>

#include <core/input.h>

b8 game_initialize(game* game_inst) {
    BOOBS_DEBUG("game_initialize() has been called");

    return true;
}

b8 game_update(game* game_inst, f32 dt) {
    static u64 alloc_count = 0;
    u64 prev_alloc_count = alloc_count;
    alloc_count = get_memory_alloc_count();

    if (input_is_key_down('M') && input_was_key_down('M')) {
        BOOBS_DEBUG("allocations %llu (%llu this frame)", alloc_count, alloc_count - prev_alloc_count);
    }

    return true;
}

b8 game_render(game* game_inst, f32 dt) {
    return true;
}

void game_on_resize(game* game_inst, u32 width, u32 height) {}
