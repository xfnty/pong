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

    game->was_initialized = true;
    game->is_running = true;

    LOG("initialized");

    screen_stack_push(&game->screen_stack, gameplay_screen_create());
    screen_stack_push(&game->screen_stack, gameplay_screen_create());

    return true;
}

void game_tick(game_t* game, update_context_t ctx) {
    assert(game->was_initialized);

    ClearBackground(BLUE);
    screen_stack_update(&game->screen_stack, ctx);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        screen_stack_push(&game->screen_stack, gameplay_screen_create());
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        screen_stack_pop(&game->screen_stack);

    game->is_running = !WindowShouldClose();
}

void game_debug_draw(game_t* game, update_context_t ctx) {
}

void game_shutdown(game_t* game) {
    assert(game->was_initialized);

    screen_stack_destroy(&game->screen_stack);
    UnloadRenderTexture(game->canvas);
    game->was_initialized = false;

    LOG("shutdown");
}
