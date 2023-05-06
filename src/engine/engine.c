#include <engine/engine.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <raylib.h>
#include <raygui.h>

#include <system/strid.h>
#include <system/debug.h>
#include <system/memory.h>
#include <system/defines.h>
#include <system/logging.h>


void _raylib_log_callback(int logLevel, const char *text, va_list args);
bool _parse_args(engine_t* engine_state, int argc, const char** argv);

bool engine_pre_init(engine_t* engine_state, int argc, const char** argv) {
    assert(logging_init());
    assert(strid_init());

	*engine_state = (engine_t) { 0 };

	if (!_parse_args(engine_state, argc, argv)) {
		engine_state->last_error = 1;
		return false;
	}

	if (argc >= 1 && strrchr(argv[0], '/'))
		engine_state->window_name = strdup(strrchr(argv[0], '/') + 1);
	else
		engine_state->window_name = strdup("Pong");

	return true;
}

bool engine_init(engine_t* engine_state) {
	debug_attach_signal_handler();

	SetTraceLogCallback(_raylib_log_callback);
	InitWindow(800, 450, engine_state->window_name);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	engine_state->is_running = true;

	return true;
}

void engine_tick(engine_t* engine_state) {
	if (!engine_state->is_running)
		return;

	engine_state->viewport_width = GetScreenWidth();
	engine_state->viewport_height = GetScreenHeight();

	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();

	engine_state->is_running = !WindowShouldClose();
}

void engine_shutdown(engine_t* engine_state) {
	CloseWindow();
    strid_shutdown();
    logging_shutdown();

	if (engine_state->window_name)
		free(engine_state->window_name);
}


void _raylib_log_callback(int logLevel, const char *text, va_list args) {
	if (logLevel > LOG_WARNING)
		vprintf(text, args);
}

bool _parse_args(engine_t* engine_state, int argc, const char** argv) {
    return true;
}
