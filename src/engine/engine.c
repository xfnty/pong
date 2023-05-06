#include <engine/engine.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>
#include <raygui.h>

#include <system/strid.h>
#include <system/debug.h>
#include <system/memory.h>
#include <system/defines.h>
#include <system/logging.h>

#include <engine/update_context.h>


static void _raylib_log_callback(int logLevel, const char *text, va_list args);

bool _parse_args(engine_t* engine, int argc, const char** argv);

bool _engine_init(engine_t* engine, int argc, const char** argv);
bool _engine_tick(engine_t* engine);
void _engine_shutdown(engine_t* engine);

int engine_run(engine_t* engine, int argc, const char** argv) {
    if (!_engine_init(engine, argc, argv))
        return engine->exit_error;

    while (_engine_tick(engine));

    _engine_shutdown(engine);

    return engine->exit_error;
}

bool _engine_init(engine_t* engine, int argc, const char** argv) {
    assert(engine->was_initialized != true);
    assert(logging_init());
    assert(strid_init());

    *engine = (engine_t) { 0 };

    if (!_parse_args(engine, argc, argv)) {
        engine->exit_error = 1;
        return false;
    }

	debug_attach_signal_handler();

	SetTraceLogCallback(_raylib_log_callback);
	InitWindow(800, 450, "Pong");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

    if (!game_init(&engine->game)) {
        engine->exit_error = 2;
        return false;
    }

	engine->was_initialized = true;

	return true;
}

bool _engine_tick(engine_t* engine) {
	assert(engine->was_initialized);

    Vector2 screen_size = (Vector2) { .x = GetScreenWidth(), .y = GetScreenHeight() };
    Vector2 viewport_size = (Vector2) { .x = engine->game.canvas.texture.width, .y = engine->game.canvas.texture.height };
    float scale = fmin(screen_size.x / viewport_size.x, screen_size.y / viewport_size.y);

    Vector2 virtualMouse = { 0 };
    virtualMouse.x = (GetMouseX() - (screen_size.x - (viewport_size.x * scale)) / 2) / scale;
    virtualMouse.y = (GetMouseY() - (screen_size.y - (viewport_size.y * scale)) / 2) / scale;
    virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, viewport_size);

    Rectangle sourceRec = { 0.0f, 0.0f, viewport_size.x, -viewport_size.y };
    Rectangle destRec = (Rectangle) {
        (screen_size.x - viewport_size.x * scale) / 2,
        (screen_size.y - viewport_size.y * scale) / 2,
        viewport_size.x * scale,
        viewport_size.y * scale
    };

    update_context_t ctx = (update_context_t) {
        .mouse = virtualMouse
    };

    BeginTextureMode(engine->game.canvas);
        game_tick(&engine->game, ctx);
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            engine->game.canvas.texture,
            sourceRec,
            destRec,
            (Vector2) { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    EndDrawing();

	return engine->game.is_running;
}

void _engine_shutdown(engine_t* engine) {
    assert(engine->was_initialized);
    
    game_shutdown(&engine->game);

	CloseWindow();
    strid_shutdown();
    logging_shutdown();

    engine->was_initialized = false;
    engine->exit_error = 0;
}


void _raylib_log_callback(int logLevel, const char *text, va_list args) {
	if (logLevel > LOG_WARNING)
		vprintf(text, args);
}

bool _parse_args(engine_t* engine, int argc, const char** argv) {
    return true;
}
