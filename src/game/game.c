#include <game/game.h>

#include <assert.h>

#define SCOPE_NAME "game"
#include <system/logging.h>
#include <system/strid.h>

#include <game/states/test.h>


void _game_switch_state(game_t* game, game_state_t new_state);

bool game_init(game_t* game) {
    assert(game != NULL);
    assert(game->was_initialized != true);

    *game = (game_t) {
        .was_initialized = true,
        .is_running = true,
        .state = (game_state_t) { 0 },
        .canvas = LoadRenderTexture(80, 50),
    };

    while (!IsTextureReady(game->canvas.texture));

    LOG("initialized");

    _game_switch_state(game, test_state_create());

    return true;
}

void game_tick(game_t* game, update_context_t ctx) {
    assert(game != NULL);
    assert(game->was_initialized);

    game->is_running = !WindowShouldClose();

    ClearBackground(BLACK);
    game_state_update(&game->state, game, ctx);
}

void game_debug_draw(game_t* game, update_context_t ctx) {
}

void game_shutdown(game_t* game) {
    assert(game != NULL);
    assert(game->was_initialized);

    game_state_exit(&game->state, game);
    UnloadRenderTexture(game->canvas);
    game->was_initialized = false;

    LOG("shutdown");
}

void _game_switch_state(game_t* game, game_state_t new_state) {
    assert(game != NULL);
    assert(game->was_initialized);

    game_state_exit(&game->state, game);
    game->state = new_state;
    game_state_enter(&game->state, game);

    LOGF("entered state \"%s\"", strid_get_str(new_state.name));
}
