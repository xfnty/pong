#include <game/states/about.h>

#include <assert.h>
#include <math.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "about"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/main_menu.h>


static void _about_state_enter(game_state_t* state, game_t* game) {
}

static void _about_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    ClearBackground(BLACK);

    DrawText("Pong", 2, 2, 8, WHITE);

    DrawText("Controls:", 2, 20, 8, WHITE);
    DrawText("  W-S - left pad", 2, 30, 8, WHITE);
    DrawText("  UP-DOWN - right pad", 2, 40, 8, WHITE);
    DrawText("  ESC - return to main menu", 2, 50, 8, WHITE);

    DrawText("> return", 2, 80, 8, WHITE);

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)) {
        game_switch_state(game, main_menu_state_create());
    }
}

static void _about_state_exit(game_state_t* state, game_t* game) {
}

game_state_t about_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _about_state_enter,
        .on_update = _about_state_update,
        .on_exit = _about_state_exit
    };
}
