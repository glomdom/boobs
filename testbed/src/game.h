#pragma once

#include <defines.h>
#include <game_types.h>

typedef struct game_state {
    f32 dt;
} game_state;

b8 game_initialize(game* game_inst);
b8 game_update(game* game_inst, f32 dt);
b8 game_render(game* game_inst, f32 dt);
void game_on_resize(game* game_inst, u32 width, u32 height);