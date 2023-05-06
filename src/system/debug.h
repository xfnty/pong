#ifndef SYSTEM_ERROR_H
#define SYSTEM_ERROR_H

#include <stddef.h>

#ifdef __GNUC__
#define _GNU_SOURCE
#endif
#include <signal.h>


typedef struct {
	const char* func_name;
	void* func_addr;
	const char* lib_file;
	void* lib_addr;
} callstack_entry_t;

typedef enum {
	CRASH_TYPE_USER_ERROR,
	CRASH_TYPE_SIGNAL,
} crash_type_t;

typedef struct {
	crash_type_t crash_type;
	union {
		struct {
			const char* message;
			const char* file;
			int line;
		} user_error;
		struct {
			int signal_id;
			const char* signal_name;
		} system_error;
	};
} crash_report_t;

typedef void (*crash_handler_f)(crash_report_t, callstack_entry_t*, size_t);

void debug_attach_signal_handler();
void debug_set_crash_handler(crash_handler_f handler);
void debug_detach_signal_handler();

size_t debug_get_callstack(callstack_entry_t* callstack, size_t callstack_size);
void debug_print_callstack(callstack_entry_t* callstack, size_t callstack_size);
void debug_print_crash_report(crash_report_t report);

void debug_crash(const char* msg, const char* file, int line);

#define DEBUG_CRASH(msg) debug_crash(msg, __FILE__, __LINE__)

#ifdef __GNUC__
#define DEBUG_BREAK() raise(SIGTRAP)
#else
#define DEBUG_BREAK() __debugbreak()
#endif

#define ASSERT_CRASH(cond, msg) do { if (!(cond)) CRASH(msg); } while(0)
#define ASSERT_BREAK(cond) do { if (!(cond)) BREAKPOINT(); } while(0)


#endif
