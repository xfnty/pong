#include <game/states/gameplay.h>

#include <assert.h>
#include <math.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "gameplay"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/main_menu.h>


static void _gameplay_state_enter(game_state_t* state, game_t* game) {
}

static void _gameplay_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    ClearBackground(BLACK);

    DrawText("0:0", game->canvas.texture.width / 2 - 8, 0, 8, WHITE);

    if (IsKeyPressed(KEY_ESCAPE)) {
        game_switch_state(game, main_menu_state_create());
    }
}

static void _gameplay_state_exit(game_state_t* state, game_t* game) {
}

game_state_t gameplay_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _gameplay_state_enter,
        .on_update = _gameplay_state_update,
        .on_exit = _gameplay_state_exit
    };
}
