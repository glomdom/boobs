#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if BOOBS_RELEASE == 1
#   define LOG_DEBUG_ENABLED 0
#   define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
} log_level;

b8 initialize_logging();
void shutdown_logging();

BOOBS_API void log_output(log_level level, const char* message, ...);

#define BOOBS_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef BOOBS_ERROR
#   define BOOBS_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#   define BOOBS_WARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#   define BOOBS_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#   define BOOBS_INFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#   define BOOBS_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#   define BOOBS_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#   define BOOBS_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#   define BOOBS_TRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#   define BOOBS_TRACE(message, ...)
#endif
