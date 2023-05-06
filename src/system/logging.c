#include <system/logging.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <kvec.h>

#include <system/defines.h>

#define ANSI_RESET			"\033[00m"
#define ANSI_COLOR_WHITE	"\033[37m"
#define ANSI_COLOR_GREEN	"\033[32m"


// TODO: Queue log entries

static kvec_t(log_callback_f) s_callbacks;
static bool s_initialized = false;

void _log_entry_destroy(log_entry_t* entry);
void _console_callback(log_entry_t entry);

bool logging_init() {
    kv_init(s_callbacks);
    s_initialized = true;
    logging_register(_console_callback);
	return true;
}

void logging_shutdown() {
    assert(s_initialized);
    
    kv_destroy(s_callbacks);
    kv_init(s_callbacks);
    s_initialized = false;
}

// void logging_flush() {
// }

void logging_register(log_callback_f consumer) {
    assert(s_initialized);

    int last_null_i = -1;
    for (size_t i = 0; i < kv_size(s_callbacks); i++) {
        if (kv_A(s_callbacks, i) == NULL) {
            last_null_i = i;
        }
        else if (kv_A(s_callbacks, i) == consumer) {
            return;
        }
    }

    if (last_null_i != -1) {
        kv_A(s_callbacks, last_null_i) = consumer;
    }
    else {
        kv_push(log_callback_f, s_callbacks, consumer);
    }
}

void logging_unregister(log_callback_f consumer) {
    assert(s_initialized);
    
    for (size_t i = 0; i < kv_size(s_callbacks); i++) {
        if (kv_A(s_callbacks, i) == consumer) {
            kv_A(s_callbacks, i) = NULL;
            return;
        }
    }
}

void _logging_push_entry(log_entry_t entry) {
    assert(s_initialized);
    
	entry.time = (log_time_t) {
		time(NULL), clock()
	};

	for (size_t i = 0; i < kv_size(s_callbacks); i++)
        if (kv_A(s_callbacks, i) != NULL)
            kv_A(s_callbacks, i)(entry);
	
    _log_entry_destroy(&entry);
}

log_msg_t _logging_create_msg(const char* restrict scope, const char* restrict txt) {
    assert(s_initialized);
    
	return (log_msg_t){ 
		scope,
		strdup(txt)
	};
}

log_msg_t _logging_create_msg_f(const char* restrict scope, const char* restrict fmt, ...) {
    assert(s_initialized);
    
	log_msg_t msg = (log_msg_t){
		scope, malloc(256)
	};

	va_list args;
	va_start(args, fmt);
	assert(vsprintf(msg.buffer, fmt, args) > 0);
	va_end(args);

	return msg;
}

void _console_callback(log_entry_t entry) {
	static char time_buffer[32];
	int bw = 0;
	memset(time_buffer, 0, STACKARRAY_SIZE(time_buffer));

	bw = snprintf(
		time_buffer, 
		STACKARRAY_SIZE(time_buffer), 
		"%s[%11.6f]%s", 
		ANSI_COLOR_GREEN, 
		(double)entry.time.cpu_time / CLOCKS_PER_SEC,
		ANSI_RESET
	);
	assert(bw > 0);

	if (entry.msg.scope) {
		printf(
			"%s %s%s%s: %s%s%s\n",
			time_buffer,
			ANSI_RESET,
			entry.msg.scope,
			ANSI_COLOR_WHITE,
			ANSI_COLOR_WHITE,
			entry.msg.buffer,
			ANSI_RESET
		);
	}
	else {
		printf(
			"%s %s%s%s\n",
			time_buffer,
			ANSI_COLOR_WHITE,
			entry.msg.buffer,
			ANSI_RESET
		);
	}
}

void _log_entry_destroy(log_entry_t* entry) {
    assert(s_initialized);
	assert(entry);
	assert(entry->msg.buffer);

	free(entry->msg.buffer);
	entry->msg.buffer = NULL;
}
