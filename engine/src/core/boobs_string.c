#include "core/boobs_string.h"
#include "core/boobs_memory.h"

#include <string.h>

u64 string_length(const char* str) {
    return strlen(str);
}

char* string_duplicate(const char* str) {
    u64 length = string_length(str);
    char* copy = boobs_allocate(length + 1, MEMORY_TAG_STRING);

    boobs_copy_memory(copy, str, length + 1);
    
    return copy;
}