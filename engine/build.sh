set echo off

cfiles=$(find . -type f -name "*.c")

assembly="engine"
cflags="-g -shared -fdeclspec -fPIC"
iflags="-Isrc -I$VULKAN_SDK/include"
ldflags="-lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$VULKAN_SDK/lib -L/usr/X11R6/lib"
defines="-D_DEBUG -DBOOBS_EXPORT -D_CRT_SECURE_NO_WARNINGS"

echo "  building $assembly"
mkdir -p ../bin
clang $cfiles $cflags -o ../bin/lib$assembly.so $defines $iflags $ldflags