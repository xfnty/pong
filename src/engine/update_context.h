#ifndef ENGINE_UPDATE_CONTEXT_H
#define ENGINE_UPDATE_CONTEXT_H

#include <raylib.h>


typedef struct update_context_s {
    Vector2 mouse; // mouse position mapped to game's viewport
} update_context_t;


#endif