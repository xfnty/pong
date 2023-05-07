#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <stdbool.h>
#include <stddef.h>

#include <raylib.h>

#include <game/state.h>


typedef struct game_s {
    bool was_initialized;
    bool is_running;
    game_state_t state;

    RenderTexture2D canvas;
} game_t;

typedef struct update_context_s {
    Vector2 mouse; // mouse position mapped to game's viewport
} update_context_t;


bool game_init(game_t* game);
void game_tick(game_t* game, update_context_t ctx);
void game_shutdown(game_t* game);

void game_debug_draw(game_t* game, update_context_t ctx);


#endif