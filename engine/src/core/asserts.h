#pragma once

#include "defines.h"

#define BOOBS_ASSERTIONS_ENABLED

#ifdef BOOBS_ASSERTIONS_ENABLED
#   if _MSC_VER
#       include <intrin.h>
#       define debugBreak() __debugbreak()
#   else
#       define debugBreak() __builtin_trap()
#   endif

BOOBS_API void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define BOOBS_ASSERT(expr) {                                        \
    if (expr) {}                                                    \
    else {                                                          \
        report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
        debugBreak();                                               \
    }                                                               \
}

#define BOOBS_ASSERT_MSG(expr, message) {                               \
    if (expr) {}                                                        \
    else {                                                              \
        report_assertion_failure(#expr, message, __FILE__, __LINE__);   \
        debugBreak();                                                   \
    }                                                                   \
}

#ifdef _DEBUG
#   define BOOBS_ASSERT_DEBUG(expr) {                                   \
        if (expr) {}                                                    \
        else {                                                          \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
            debugBreak();                                               \
        }                                                               \
    }
#else
#   define BOOBS_ASSERT_DEBUG(expr)
#endif

#else
#   define BOOBS_ASSERT(expr)
#   define BOOBS_ASSERT_MSG(expr, message)
#   define BOOBS_ASSERT_DEBUG(expr)
#endif
