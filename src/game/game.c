#include <game/game.h>

#include <assert.h>

#define SCOPE_NAME "game"
#include <system/logging.h>

#include <game/screens/gameplay.h>


bool game_init(game_t* game) {
    assert(game->was_initialized != true);

    game->canvas = LoadRenderTexture(80, 50);
    while (!IsTextureReady(game->canvas.texture));

    screen_stack_init(&game->screen_stack);
    screen_stack_push(&game->screen_stack, gameplay_screen_create());

    game->was_initialized = true;
    game->is_running = true;

    LOG("Initialized");

    return true;
}

void game_tick(game_t* game, update_context_t ctx) {
    assert(game->was_initialized);

    ClearBackground(BLUE);
    screen_stack_update(&game->screen_stack, ctx);
    DrawPixelV(ctx.mouse, RED);

    game->is_running = !WindowShouldClose();
}

void game_debug_draw(game_t* game, update_context_t ctx) {
    DrawText(TextFormat("%.1f %.1f", ctx.mouse.x, ctx.mouse.y), 2, 2, 8, YELLOW);
}

void game_shutdown(game_t* game) {
    assert(game->was_initialized);

    screen_stack_destroy(&game->screen_stack);
    UnloadRenderTexture(game->canvas);
    game->was_initialized = false;

    LOG("Shutdown");
}
