#include <system/memory.h>

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/resource.h>

#include <system/defines.h>
#include <system/debug.h>


static memory_stats_t s_memory_stats = (memory_stats_t){ 0 };

extern void* __libc_malloc(size_t size);
extern void* __libc_realloc(void* mem, size_t size);
extern void __libc_free(void* mem);

bool memory_init() {
	return true;
}

void memory_shutdown() {
}

void* memory_malloc(size_t size) {
	void* mem = __libc_malloc(size);
    assert(mem);

    size = malloc_usable_size(mem);
	s_memory_stats.currently_allocated_bytes += size;

    return mem;
}

void* memory_realloc(void* oldmem, size_t size) {
	return __libc_realloc(oldmem, size);
}

void memory_free(void* mem) {
	size_t size = malloc_usable_size(mem);
	s_memory_stats.currently_allocated_bytes -= size;

	__libc_free(mem);
}

memory_stats_t memory_get_stats() {
	return s_memory_stats;
}

void* malloc(size_t size) {
	return memory_malloc(size);
}

void* realloc(void* oldmem, size_t size) {
	return memory_realloc(oldmem, size);
}

void free(void* mem) {
	memory_free(mem);
}
