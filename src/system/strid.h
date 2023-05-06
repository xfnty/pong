#ifndef SYSTEM_STRID_H
#define SYSTEM_STRID_H

#include <system/defines.h>


typedef uint64_t strid_t;

bool strid_init();
void strid_shutdown();

strid_t strid_get_strid(const char* str);

const char* strid_get_str(strid_t sid);

bool strid_has_str(const char* str);
bool strid_has_strid(strid_t sid);


#endif