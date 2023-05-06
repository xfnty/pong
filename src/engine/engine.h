#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <stdbool.h>
#include <stddef.h>


typedef struct engine_t engine_t;
struct engine_t {
	char* window_name;
	size_t viewport_width;
	size_t viewport_height;

	bool is_running;

	unsigned int last_error;
};

bool engine_pre_init(engine_t* engine_state, int argc, const char** argv);
bool engine_init(engine_t* engine_state);
void engine_tick(engine_t* engine_state);
void engine_shutdown(engine_t* engine_state);


#endif