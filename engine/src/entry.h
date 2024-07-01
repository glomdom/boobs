#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/boobs_memory.h"
#include "game_types.h"

extern b8 create_game(game* out_game);

int main(void) {
    initialize_memory();

    game game_inst;
    if (!create_game(&game_inst)) {
        BOOBS_FATAL("failed to create game");

        return -1;
    }

    if (!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
        BOOBS_FATAL("game is missing functions");

        return -2;
    }

    if (!application_create(&game_inst)) {
        BOOBS_INFO("application failed to create");

        return 1;
    }

    if (!application_run()) {
        BOOBS_INFO("application did not shutdown gracefully");

        return 2;
    }

    shutdown_memory();

    return 0;
}