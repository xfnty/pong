#include <game/screen_stack.h>

#include <assert.h>

#include <kvec.h>

#include <system/logging.h>

#include <game/game.h>
#include <game/screen.h>


void screen_stack_init(screen_stack_t* stack) {
    assert(stack != NULL);
    kv_init(*stack);
}

void screen_stack_push(screen_stack_t* stack, screen_t screen) {
    assert(stack != NULL);
    assert(screen.type != SCREEN_TYPE_INVALID);

    kv_push(screen_t, *stack, screen);

    screen_t* scr = screen_stack_top(stack);
    if (scr->on_enter)
        scr->on_enter(scr);
}

void screen_stack_pop(screen_stack_t* stack) {
    assert(stack != NULL);

    screen_t* scr = screen_stack_top(stack);
    if (scr == NULL)
        return;
    
    if (scr->on_exit)
        scr->on_exit(scr);

    kv_pop(*stack);
}

void screen_stack_clear(screen_stack_t* stack) {
    assert(stack != NULL);

    while (kv_size(*stack) > 0)
        screen_stack_pop(stack);
    
    kv_destroy(*stack);
    kv_init(*stack);
}

void screen_stack_destroy(screen_stack_t* stack) {
    assert(stack != NULL);
    screen_stack_clear(stack);
}

void screen_stack_update(screen_stack_t* stack, update_context_t ctx) {
    assert(stack != NULL);

    // First go top to bottom of the stack
    // to find first TYPE_NORMAL screen
    // below which screens should not be updated.
    int lowest_screen_i;
    for (lowest_screen_i = kv_size(*stack) - 1; lowest_screen_i > 0; lowest_screen_i--) {
        if (kv_A(*stack, lowest_screen_i).type == SCREEN_TYPE_NORMAL)
            break;
    }

    // Second go back from bottom to top
    // starting from lowest_screen_i.
    for (int i = lowest_screen_i; i < kv_size(*stack); i++) {
        screen_t* scr = &kv_A(*stack, i);
        if (scr->on_update)
            scr->on_update(scr, ctx);
    }
}

void screen_stack_make_current(screen_stack_t* stack, strid_t name) {
    int i;
    for (i = 0; i < kv_size(*stack); i++) {
        if (kv_A(*stack, i).name == name)
            break;
    }

    for (int j = kv_size(*stack) - 1; j > i ; j--) {
        screen_stack_pop(stack);
    }
}

screen_t* screen_stack_top(screen_stack_t* stack) {
    assert(stack != NULL);
    return (kv_size(*stack) > 0) ? &(kv_A(*stack, kv_size(*stack) - 1)) : NULL;
}
