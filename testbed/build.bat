@echo off
setlocal enabledelayedexpansion

set cfiles=
for /r %%f in (*.c) do (
    set cfiles=!cfiles! %%f
)

set assembly=testbed
set cflags=-g 
set iflags=-Isrc -I../engine/src/
set ldflags=-L../bin/ -lboobs.lib
set defines=-D_DEBUG -DBOOBS_IMPORT

echo   building %assembly%
if not exist ..\bin mkdir ..\bin
clang %cfiles% %cflags% -o ../bin/%assembly%.exe %defines% %iflags% %ldflags%