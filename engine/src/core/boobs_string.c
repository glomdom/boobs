#include "core/boobs_string.h"
#include "core/boobs_memory.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

u64 string_length(const char* str) {
    return strlen(str);
}

char* string_duplicate(const char* str) {
    u64 length = string_length(str);
    char* copy = boobs_allocate(length + 1, MEMORY_TAG_STRING);

    boobs_copy_memory(copy, str, length + 1);

    return copy;
}

b8 strings_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

i32 string_format(char* dest, const char* format, ...) {
    if (dest) {
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, format);

        i32 written = string_format_v(dest, format, arg_ptr);

        va_end(arg_ptr);

        return written;
    }

    return -1;
}

i32 string_format_v(char* dest, const char* format, void* va_listp) {
    if (dest) {
        char buffer[32000];

        i32 written = vsnprintf(buffer, 32000, format, va_listp);
        buffer[written] = 0;

        boobs_copy_memory(dest, buffer, written + 1);

        return written;
    }

    return -1;
}
