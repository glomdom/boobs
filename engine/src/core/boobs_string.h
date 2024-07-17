#pragma once

#include "defines.h"

BOOBS_API u64 string_length(const char* str);
BOOBS_API char* string_duplicate(const char* str);
BOOBS_API b8 strings_equal(const char* str0, const char* str1);

BOOBS_API i32 string_format(char* dest, const char* format, ...);
BOOBS_API i32 string_format_v(char* dest, const char* format, void* va_list);
