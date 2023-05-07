#ifndef LOGC_LOG_H
#define LOGC_LOG_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <system/defines.h>

#ifndef SCOPE_NAME
#define SCOPE_NAME NULL
#endif


typedef struct {
	const char* func;
	const char* file;
	int line;
} log_pos_t;

typedef struct {
	time_t unix_time;
	clock_t cpu_time;
} log_time_t;

typedef struct {
	const char* scope;
	char* buffer;
} log_msg_t;

typedef uint8_t log_flags_t;

#define _LOGGING_FLAGS_TRACE 0x01
#define _LOGGING_FLAGS_NONE 0

typedef struct {
	log_msg_t msg;
	log_pos_t pos;
	log_time_t time;
	log_flags_t flags;
} log_entry_t;


bool logging_init();
void logging_shutdown();

typedef void (*log_callback_f)(log_entry_t);
void logging_register(log_callback_f callback);
void logging_unregister(log_callback_f callback);

void logging_flush();

void _logging_push_entry(log_entry_t entry);

log_msg_t _logging_create_msg(const char* restrict scope, const char* restrict txt);
log_msg_t _logging_create_msg_f(const char* restrict scope, const char* restrict fmt, ...);

#define _LOGGING_POS_HERE() ((log_pos_t) { __func__, __FILE__, __LINE__ })
#define _LOGGING_TIME_CURRENT() ((log_time_t) { time(NULL), clock() })

#define LOG(msg) _logging_push_entry(								\
		(log_entry_t) {											    \
			_logging_create_msg(SCOPE_NAME, msg),					\
			_LOGGING_POS_HERE(),									\
			_LOGGING_TIME_CURRENT(),								\
			_LOGGING_FLAGS_NONE 									\
		}															\
	)

#define LOGF(msg, ...) _logging_push_entry(							\
		(log_entry_t) {											    \
			_logging_create_msg_f(SCOPE_NAME, msg, __VA_ARGS__),	\
			_LOGGING_POS_HERE(),									\
			_LOGGING_TIME_CURRENT(),								\
			_LOGGING_FLAGS_NONE										\
		}															\
	)

// #define LOG_TRACE(msg) _logging_push_entry(				\
// 		(log_entry_t) {									\
// 			_logging_create_msg(NULL, msg),					\
// 			_LOGGING_POS_HERE(),							\
// 			_LOGGING_TIME_CURRENT(),						\
// 			_LOGGING_FLAGS_TRACE 							\
// 		}													\
// 	)

// #define LOGF_TRACE(msg, ...) _logging_push_entry(		\
// 		(log_entry_t) {									\
// 			_logging_create_msg_f(NULL, msg, __VA_ARGS__),	\
// 			_LOGGING_POS_HERE(),							\
// 			_LOGGING_TIME_CURRENT(),						\
// 			_LOGGING_FLAGS_TRACE 							\
// 		}													\
// 	)


#endif
