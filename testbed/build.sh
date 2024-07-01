set echo off

cfiles=$(find . -type f -name "*.c")

assembly="testbed"
cflags="-g -fdeclspec -fPIC"
iflags="-Isrc -I../engine/src"
ldflags=" -lboobs -Wl,-rpath,. -L../bin/"
defines="-D_DEBUG -DBOOBS_IMPORT"

echo "  building $assembly"
mkdir -p ../bin
clang $cfiles $cflags -o ../bin/$assembly $defines $iflags $ldflags
