#include <game/states/intro.h>

#include <assert.h>
#include <math.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "intro"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/main_menu.h>


typedef struct intro_state_s {
    double start_time;
} intro_state_t;

static void _intro_state_enter(game_state_t* state, game_t* game) {
    state->data = malloc(sizeof(intro_state_t));
    assert(state->data);
    intro_state_t* intro = (intro_state_t*)state->data;

    *intro = (intro_state_t) {
        .start_time = GetTime()
    };
}

static void _intro_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    intro_state_t* intro = (intro_state_t*)state->data;

    float progress = (GetTime() - intro->start_time) / 1.5;
    if (GetKeyPressed() != 0 || progress >= 1)
        game_switch_state(game, main_menu_state_create());

    int size = 40;
    Rectangle rect = (Rectangle) {
        .x = (game->canvas.texture.width - size) / 2,
        .y = (game->canvas.texture.height - size) / 2,
        .width = size,
        .height = size
    };

    DrawText("Made with", 0, (game->canvas.texture.height) / 2 - 9, 8, WHITE);
    DrawRectangleLinesEx(rect, 2, WHITE);
    DrawText(TextSubtext("raylib", 0, 6 * fmin(1, progress / 0.15)), rect.x + 8, rect.y + 27, 8, WHITE);
}

static void _intro_state_exit(game_state_t* state, game_t* game) {
    free(state->data);
}

game_state_t intro_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _intro_state_enter,
        .on_update = _intro_state_update,
        .on_exit = _intro_state_exit
    };
}
