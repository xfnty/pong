#include <game/screens/gameplay.h>

#include <raylib.h>

#define SCOPE_NAME "gameplay"
#include <system/logging.h>
#include <system/strid.h>

#include <game/game.h>


static void gameplay_screen_on_enter(screen_t* scr);
static void gameplay_screen_on_update(screen_t* scr, update_context_t ctx);
static void gameplay_screen_on_exit(screen_t* scr);

screen_t gameplay_screen_create() {
    return (screen_t) {
        .name = strid_get_strid("gameplay"),
        .type = SCREEN_TYPE_NORMAL,
        .on_enter = gameplay_screen_on_enter,
        .on_update = gameplay_screen_on_update,
        .on_exit = gameplay_screen_on_exit
    };
}

static void gameplay_screen_on_enter(screen_t* scr) {
    LOG("entered");
}

static void gameplay_screen_on_update(screen_t* scr, update_context_t ctx) {
    DrawPixelV(ctx.mouse, RED);
}

static void gameplay_screen_on_exit(screen_t* scr) {
    LOG("exited");
}
