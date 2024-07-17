#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct logger_system_state {
    b8 initialized;
} logger_system_state;

static logger_system_state* state_ptr;

b8 initialize_logging(u64* memory_requirement, void* state) {
    *memory_requirement = sizeof(logger_system_state);
    if (state == 0) {
        return true;
    }

    state_ptr = state;
    state_ptr->initialized = true;

    return true;
}

void shutdown_logging(void* state) {
    state_ptr = 0;
}

void log_output(log_level level, const char* message, ...) {
    b8 is_error = level < LOG_LEVEL_WARN;

    char out_message[MESSAGE_LENGTH];
    memset(out_message, 0, sizeof(out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, MESSAGE_LENGTH, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[MESSAGE_LENGTH];
    sprintf(out_message2, "%s\n", out_message);

    if (is_error) {
        platform_console_write_error(out_message2, level);
    } else {
        platform_console_write(out_message2, level);
    }
}

void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "boobs assertion failure: %s, '%s' at %s:%d\n", expression, message, file, line);
}
