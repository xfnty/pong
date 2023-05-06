#include <game/game.h>

#include <assert.h>

#define SCOPE_NAME "game"
#include <system/logging.h>


bool game_init(game_t* game) {
    assert(game->was_initialized != true);

    game->canvas = LoadRenderTexture(858, 525);
    while (!IsTextureReady(game->canvas.texture));

    game->was_initialized = true;
    game->is_running = true;

    LOG("Initialized");

    return true;
}

void game_tick(game_t* game, update_context_t ctx) {
    assert(game->was_initialized);
    
    ClearBackground(BLUE);

    game->is_running = !WindowShouldClose();
}

void game_shutdown(game_t* game) {
    assert(game->was_initialized);

    UnloadRenderTexture(game->canvas);
    game->was_initialized = false;

    LOG("Shutdown");
}
