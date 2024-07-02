@echo off
setlocal enabledelayedexpansion

set cfiles=
for /r %%f in (*.c) do (
    set cfiles=!cfiles! %%f
)

set assembly=boobs
set cflags=-g -shared -Wvarargs -Wall
set iflags=-Isrc -I%VULKAN_SDK%\Include
set ldflags=-luser32 -lvulkan-1 -L%VULKAN_SDK%\Lib
set defines=-D_DEBUG -DBOOBS_EXPORT -D_CRT_SECURE_NO_WARNINGS

echo   building %assembly%
if not exist ..\bin mkdir ..\bin
clang %cfiles% %cflags% -o ../bin/%assembly%.dll %defines% %iflags% %ldflags%