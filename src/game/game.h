#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <stdbool.h>
#include <stddef.h>

#include <raylib.h>

#include <game/screen_stack.h>


typedef struct game_s {
    bool was_initialized;
    bool is_running;

    RenderTexture2D canvas;
    screen_stack_t screen_stack;
} game_t;

typedef struct update_context_s {
    game_t* game;
    Vector2 mouse; // mouse position mapped to game's viewport
} update_context_t;


bool game_init(game_t* game);
void game_tick(game_t* game, update_context_t ctx);
void game_shutdown(game_t* game);

void game_debug_draw(game_t* game, update_context_t ctx);


#endif