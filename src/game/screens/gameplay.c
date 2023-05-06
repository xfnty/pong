#include <game/screens/gameplay.h>

#include <raylib.h>

#define SCOPE_NAME "gameplay"
#include <system/logging.h>

#include <engine/update_context.h>


static void gameplay_screen_on_enter(screen_t* scr);
static void gameplay_screen_on_update(screen_t* scr, update_context_t ctx);
static void gameplay_screen_on_exit(screen_t* scr);

screen_t gameplay_screen_create() {
    static int64_t cnt = 0;

    return (screen_t) {
        .name = "gameplay",
        .type = SCREEN_TYPE_POPUP,
        .data = (void*)cnt++,
        .on_enter = gameplay_screen_on_enter,
        .on_update = gameplay_screen_on_update,
        .on_exit = gameplay_screen_on_exit
    };
}

static void gameplay_screen_on_enter(screen_t* scr) {
    LOGF("entered %lli", (int64_t)scr->data);
}

static void gameplay_screen_on_update(screen_t* scr, update_context_t ctx) {
    static Color cls[] = { RED, YELLOW };
    int64_t i = (int64_t)scr->data;
    DrawRectangle(2 + 3*i, 2 + 3*i, 12, 5, cls[i%2]);
}

static void gameplay_screen_on_exit(screen_t* scr) {
    LOGF("exited %lli", (int64_t)scr->data);
}
