#include "game.h"

#include <entry.h>
#include <core/boobs_memory.h>

b8 create_game(game* out_game) {
    out_game->app_config.start_pos_x = 100;
    out_game->app_config.start_pos_y = 100;
    out_game->app_config.start_width = 1280;
    out_game->app_config.start_height = 720;
    out_game->app_config.name = "boobs engine test";

    out_game->update = game_update;
    out_game->render = game_render;
    out_game->initialize = game_initialize;
    out_game->on_resize = game_on_resize;

    out_game->state = boobs_allocate(sizeof(game_state), MEMORY_TAG_GAME);

    return TRUE;
}