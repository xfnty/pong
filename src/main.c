#include <engine/engine.h>


int main(int argc, const char** argv) {
	engine_t engine_state;

	if (!engine_pre_init(&engine_state, argc, argv))
		return engine_state.last_error;

	if (!engine_init(&engine_state))
		return engine_state.last_error;

	while (engine_state.is_running)
		engine_tick(&engine_state);

	engine_shutdown(&engine_state);

	return 0;
}
