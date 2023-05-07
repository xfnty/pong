#include <game/states/main_menu.h>

#include <assert.h>
#include <math.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "main menu"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/test.h>


typedef struct main_menu_state_s {
} main_menu_state_t;

static void _main_menu_state_enter(game_state_t* state, game_t* game) {
}

static void _main_menu_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    ClearBackground(BLUE);
    DrawPixelV(ctx.mouse, RED);

    if (IsKeyPressed(KEY_SPACE))
        game_switch_state(game, test_state_create());
}

static void _main_menu_state_exit(game_state_t* state, game_t* game) {
}

game_state_t main_menu_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _main_menu_state_enter,
        .on_update = _main_menu_state_update,
        .on_exit = _main_menu_state_exit
    };
}
