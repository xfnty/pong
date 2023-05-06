#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <stdbool.h>
#include <stddef.h>

#include <raylib.h>

#include <engine/update_context.h>


typedef struct game_s {
    bool was_initialized;
    bool is_running;

    RenderTexture2D canvas;
} game_t;


bool game_init(game_t* game);
void game_tick(game_t* game, update_context_t ctx);
void game_shutdown(game_t* game);


#endif