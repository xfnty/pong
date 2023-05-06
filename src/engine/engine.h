#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <stdbool.h>
#include <stddef.h>

#include <game/game.h>


typedef struct engine_t engine_t;
struct engine_t {
	bool was_initialized;
	int exit_error;
    game_t game;
};

int engine_run(engine_t* engine_state, int argc, const char** argv);


#endif