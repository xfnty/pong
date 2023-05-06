#include <engine/engine.h>


int main(int argc, const char** argv) {
	engine_t engine_state;
	return engine_run(&engine_state, argc, argv);
}
