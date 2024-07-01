#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef int b32;
typedef char b8;

#if defined(__clang__) || defined(__gcc__)
#   define STATIC_ASSERT _Static_assert
#else
#   define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT(sizeof(u8) == 1, "boobs expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "boobs expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "boobs expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "boobs expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "boobs expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "boobs expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "boobs expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "boobs expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "boobs expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "boobs expected f64 to be 8 bytes.");

#define TRUE 1
#define FALSE 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define BOOBS_PLATFORM_WINDOWS 1
#   ifndef _WIN64
#       error "64-bit is required on Windows!"
#   endif
#elif defined(__linux__) || defined(__gnu_linux__)
#   define BOOBS_PLATFORM_LINUX 1
#   if defined(__ANDROID__)
#       define BOOBS_PLATFORM_ANDROID 1
#   endif
#elif defined(__unix__)
#   define BOOBS_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#   define BOOBS_PLATFORM_POSIX 1
#elif __APPLE__
#   define BOOBS_PLATFORM_APPLE 1
#   include <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR
#       define BOOBS_PLATFORM_IOS 1
#       define BOOBS_PLATFORM_IOS_SIMULATOR 1
#   elif TARGET_OS_IPHONE
#       define BOOBS_PLATFORM_IOS 1
#   elif TARGET_OS_MAC
#   else
#       error "boobs unknown apple platform"
#   endif
#else
#   error "boobs unknown platform"
#endif

#ifdef BOOBS_EXPORT
#   ifdef _MSC_VER
#       define BOOBS_API __declspec(dllexport)
#   else
#       define BOOBS_API __attribute__((visibility("default")))
#   endif
#else
#   ifdef _MSC_VER
#       define BOOBS_API __declspec(dllimport)
#   else
#       define BOOBS_API
#   endif
#endif
