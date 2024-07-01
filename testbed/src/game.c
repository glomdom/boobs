#include "game.h"

#include <core/logger.h>

b8 game_initialize(game* game_inst) {
    BOOBS_DEBUG("game_initialize() has been called");

    return TRUE;
}

b8 game_update(game* game_inst, f32 dt) {
    return TRUE;
}

b8 game_render(game* game_inst, f32 dt) {
    return TRUE;
}

void game_on_resize(game* game_inst, u32 width, u32 height) {}