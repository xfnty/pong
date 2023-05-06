#include <system/debug.h>

#define _GNU_SOURCE
#include <execinfo.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <system/defines.h>

#define USER_CRASH_SIGNAL SIGUSR1


static volatile sig_atomic_t s_crash_handling_in_progress = 0;
static crash_handler_f s_user_crash_handler = NULL;
static crash_report_t s_crash_report;

void _signal_handler(int sig);
void _set_common_signals_handler(void* handler);

size_t debug_get_callstack(callstack_entry_t* callstack, size_t callstack_size) {
	void* backtrace_buffer[128];
	size_t backtrace_size = backtrace(backtrace_buffer, STACKARRAY_SIZE(backtrace_buffer));

	size_t i = 0;
	for (i = 0; i < backtrace_size && i < callstack_size; i++) {
		Dl_info info;
		int ok = dladdr(backtrace_buffer[i], &info);
		if (!ok)
			continue;

		callstack[i].func_name = info.dli_sname;
		callstack[i].func_addr = backtrace_buffer[i];
		callstack[i].lib_file =  "";
		callstack[i].lib_addr = info.dli_fbase;
	}

	return i;
}

void debug_print_callstack(callstack_entry_t* callstack, size_t callstack_size) {
	printf("Callstack:\n");

	if (callstack_size == 0) {
		printf("empty\n");
		return;
	}

	for (size_t i = 0; i < callstack_size; i++) {
		if (callstack[i].func_name) {
			printf("  %s()\n", callstack[i].func_name);
		}
		else {
			printf("  [%p]\n", callstack[i].func_addr);
		}
	}
}

void debug_attach_signal_handler() {
	_set_common_signals_handler(_signal_handler);
	signal(USER_CRASH_SIGNAL, _signal_handler);
}

void debug_detach_signal_handler() {
	_set_common_signals_handler(SIG_DFL);
	signal(USER_CRASH_SIGNAL, SIG_DFL);
}

void debug_set_crash_handler(crash_handler_f handler) {
	s_user_crash_handler = handler;
}

void debug_crash(const char* msg, const char* file, int line) {
	s_crash_report.crash_type = CRASH_TYPE_USER_ERROR;
	s_crash_report.user_error.message = msg;
	s_crash_report.user_error.file = strrchr(file, '/') + 1;
	s_crash_report.user_error.line = line;

	raise(USER_CRASH_SIGNAL);
}

void debug_print_crash_report(crash_report_t report) {
	if (report.crash_type == CRASH_TYPE_USER_ERROR) {
		printf(
			"Crash: %s (at %s:%d)\n",
			report.user_error.message,
			report.user_error.file,
			report.user_error.line
		);
	}
	else if (report.crash_type == CRASH_TYPE_SIGNAL) {
		printf(
			"Crash: %s (%d)\n",
			report.system_error.signal_name,
			report.system_error.signal_id
		);
	}
}

void _signal_handler(int sig) {
	// if (s_crash_handling_in_progress)
	//     return;
	// s_crash_handling_in_progress = 1;

	// Leave "^C" on a separate line
	printf("\n");
	fflush(stdout);

	callstack_entry_t callstack[128];
	size_t callstack_size = debug_get_callstack(callstack, STACKARRAY_SIZE(callstack));

	if (sig == USER_CRASH_SIGNAL && s_user_crash_handler) {
		s_user_crash_handler(s_crash_report, callstack, callstack_size);
	}
	else {
		s_crash_report.crash_type = CRASH_TYPE_SIGNAL;
		s_crash_report.system_error.signal_id = sig;
		s_crash_report.system_error.signal_name =
			(sig == USER_CRASH_SIGNAL)
			? "Unhandled user error"
			: strsignal(sig);

		debug_print_crash_report(s_crash_report);
		debug_print_callstack(callstack, callstack_size);
	}

	// s_crash_handling_in_progress = 0;

	exit(sig);
}

void _set_common_signals_handler(void* handler) {
	signal(SIGSEGV, handler);
	signal(SIGABRT, handler);
	signal(SIGFPE, handler);
	signal(SIGINT, handler);
	signal(SIGKILL, handler);
	signal(SIGILL, handler);
	signal(SIGBUS, handler);
	signal(SIGSYS, handler);
	signal(SIGTERM, handler);
	signal(SIGQUIT, handler);
}
