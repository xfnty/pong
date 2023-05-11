#include <game/states/main_menu.h>

#include <assert.h>
#include <math.h>

#include <raylib.h>
#include <raygui.h>

#include <system/strid.h>
#define SCOPE_NAME "main menu"
#include <system/logging.h>

#include <game/game.h>
#include <game/states/gameplay.h>
#include <game/states/about.h>


typedef struct main_menu_state_s {
    int selected_menu_item;
} main_menu_state_t;

static void _main_menu_state_enter(game_state_t* state, game_t* game) {
    state->data = malloc(sizeof(main_menu_state_t));
    assert(state->data);
    main_menu_state_t* menu = (main_menu_state_t*)state->data;
    *menu = (main_menu_state_t) {
        .selected_menu_item = 0
    };
}

static void _main_menu_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    main_menu_state_t* menu = (main_menu_state_t*)state->data;

    ClearBackground(BLACK);

    DrawText("Pong", 2, 2, 8, WHITE);

    const int menu_count = 3;
    DrawText((menu->selected_menu_item == 0) ? "> play" : "  play", (menu->selected_menu_item == 0) + 1, 50, 8, (menu->selected_menu_item == 0) ? WHITE : GRAY);
    DrawText((menu->selected_menu_item == 1) ? "> about" : "  about", (menu->selected_menu_item == 1) + 1, 60, 8, (menu->selected_menu_item == 1) ? WHITE : GRAY);
    DrawText((menu->selected_menu_item == 2) ? "> exit" : "  exit", (menu->selected_menu_item == 2) + 1, 70, 8, (menu->selected_menu_item == 2) ? WHITE : GRAY);

    menu->selected_menu_item += (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) - (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W));
    menu->selected_menu_item = (menu->selected_menu_item >= menu_count) ? 0 : ((menu->selected_menu_item < 0) ? menu_count - 1 : menu->selected_menu_item);

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (menu->selected_menu_item) {
        case 0:
            game_switch_state(game, gameplay_state_create());
            break;

        case 1:
            game_switch_state(game, about_state_create());
            break;

        case 2:
            game->is_running = false;
            break;

        default:
            LOGF("unknown menu->selected_menu_item (%d)", menu->selected_menu_item);
        }
    }
}

static void _main_menu_state_exit(game_state_t* state, game_t* game) {
}

game_state_t main_menu_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _main_menu_state_enter,
        .on_update = _main_menu_state_update,
        .on_exit = _main_menu_state_exit
    };
}
