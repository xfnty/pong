#include <game/states/test.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "test"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/intro.h>


static void _test_state_enter(game_state_t* state, game_t* game) {
}

static void _test_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    ClearBackground(BLACK);
    DrawPixelV(ctx.mouse, RED);

    if (IsKeyPressed(KEY_SPACE))
        game_switch_state(game, intro_state_create());
}

static void _test_state_exit(game_state_t* state, game_t* game) {
}

game_state_t test_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _test_state_enter,
        .on_update = _test_state_update,
        .on_exit = _test_state_exit
    };
}
