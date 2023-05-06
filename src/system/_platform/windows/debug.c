#include <system/debug.h>


size_t debug_get_callstack(callstack_entry_t* callstack, size_t callstack_size) {
	return 0;
}

void debug_print_callstack(callstack_entry_t* callstack, size_t callstack_size) {
}

void debug_attach_signal_handler() {
}

void debug_detach_signal_handler() {
}

void debug_set_crash_handler(crash_handler_f handler) {
}

void debug_crash(const char* msg, const char* file, int line) {
}

void debug_print_crash_report(crash_report_t report) {
}
