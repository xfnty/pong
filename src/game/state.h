#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <system/strid.h>


typedef struct game_s game_t;
typedef struct game_state_s game_state_t;
typedef struct update_context_s update_context_t;

typedef struct game_state_s {
    strid_t name;
    void* data;

    void (*on_enter)(game_state_t*, game_t*);
    void (*on_update)(game_state_t*, game_t*, update_context_t);
    void (*on_exit)(game_state_t*, game_t*);
} game_state_t;


void game_state_enter(game_state_t* state, game_t* game);
void game_state_update(game_state_t* state, game_t* game, update_context_t ctx);
void game_state_exit(game_state_t* state, game_t* game);


#endif