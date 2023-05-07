#include <game/state.h>

#include <assert.h>

#include <system/strid.h>
#define SCOPE_NAME "game state manager"
#include <system/logging.h>

#include <game/game.h>


void game_state_enter(game_state_t* state, game_t* game) {
    assert(state != NULL && game != NULL);

    if (state->on_enter)
        state->on_enter(state, game);
}

void game_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    assert(state != NULL && game != NULL);

    if (state->on_update)
        state->on_update(state, game, ctx);
}

void game_state_exit(game_state_t* state, game_t* game) {
    assert(state != NULL && game != NULL);

    if (state->on_exit)
        state->on_exit(state, game);
}
