#ifndef GAME_SCREEN_STACK_H
#define GAME_SCREEN_STACK_H

#include <kvec.h>

#include <system/strid.h>


typedef struct game_s game_t;
typedef struct screen_s screen_t;
typedef kvec_t(screen_t) screen_stack_t;
typedef struct update_context_s update_context_t;

void screen_stack_init(screen_stack_t* stack);
void screen_stack_push(screen_stack_t* stack, screen_t screen);
void screen_stack_pop(screen_stack_t* stack);
void screen_stack_clear(screen_stack_t* stack);
void screen_stack_destroy(screen_stack_t* stack);

screen_t* screen_stack_top(screen_stack_t* stack);
void screen_stack_make_current(screen_stack_t* stack, strid_t name);
void screen_stack_update(screen_stack_t* stack, update_context_t ctx);



#endif