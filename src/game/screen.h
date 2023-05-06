#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <kvec.h>

#include <engine/update_context.h>


typedef struct game_s game_t;
typedef struct screen_s screen_t;

typedef enum {
    SCREEN_TYPE_INVALID,
    SCREEN_TYPE_NORMAL,  // Completely hides and stops underlying screen
    SCREEN_TYPE_POPUP,   // Just dims the underlying screen
} screen_type_t;

typedef struct screen_s {
    const char* name;
    screen_type_t type;
    void* data;

    void (*on_enter)(screen_t*);
    void (*on_update)(screen_t*, update_context_t);
    void (*on_exit)(screen_t*);
} screen_t;


#endif