#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <kvec.h>


typedef struct game_s game_t;
typedef struct screen_s screen_t;

typedef enum {
    SCREEN_TYPE_INVALID,
    SCREEN_TYPE_NORMAL,  // Completely hides and stops underlying screen
    SCREEN_TYPE_POPUP,   // Just dims the underlying screen
} screen_type_t;

typedef void (*screen_callback_f)(screen_t*, game_t*);

typedef struct screen_s {
    screen_type_t type;
    void* data;

    screen_callback_f on_enter;
    screen_callback_f on_update;
    screen_callback_f on_exit;
} screen_t;


#endif