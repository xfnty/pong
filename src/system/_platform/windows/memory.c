#include <system/memory.h>


bool memory_init() {
	return true;
}

void memory_shutdown() {
}

void* memory_malloc(size_t size) {
	return malloc(size);
}

void* memory_realloc(void* oldmem, size_t size) {
	return realloc(oldmem, size);
}

void memory_free(void* mem) {
	free(mem);
}

memory_stats_t memory_get_stats() {
	return (memory_stats_t){ 0 };
}
