#ifndef SYSTEM_MEMORY_H
#define SYSTEM_MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <system/defines.h>


typedef struct {
	size_t currently_allocated_bytes;
} memory_stats_t;

bool memory_init();
void memory_shutdown();

void* memory_malloc(size_t size);
void* memory_realloc();
void memory_free();

memory_stats_t memory_get_stats();


#endif