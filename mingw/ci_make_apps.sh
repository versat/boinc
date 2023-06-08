#!/bin/sh
set -e

if [ ! -d "mingw" ]; then
    echo "start this script in the source root directory"
    exit 1
fi

CACHE_DIR="$PWD/3rdParty/buildCache/mingw"
BUILD_DIR="$PWD/3rdParty/mingw"
VCPKG_ROOT="$BUILD_DIR/vcpkg"
export VCPKG_DIR="$VCPKG_ROOT/installed/x64-mingw-static"
export CXX="x86_64-w64-mingw32-g++"
export CC="x86_64-w64-mingw32-gcc"
# condor
export MINGW_FLAGS="-Dflockfile=_lock_file -Dfunlockfile=_unlock_file"
export CURL_EXTRA_LDFLAGS="-lcurl -lwinmm -lpthread -lssl -lcrypto -lws2_32 -lzlib -ladvapi32 -lcrypt32"
# wrapper
export MINGW_WRAPPER_FLAGS="-DEINSTEINATHOME_CROSS_BUILD -DMINGW_WIN32 -DHAVE_STRCASECMP -D_WINDOWS -D_WIN32 -DWIN32 -DWINVER=0x0500 -D_WIN32_WINNT=0x0500 -D_MT -DBOINC -DNODB -D_CONSOLE -fexceptions"
export MINGW_ZIP_FIX="../../zip/zip/__p___mb_cur_max.c"
export MINGW_LIBS="-lpsapi"
# wrappture
if [ ! -f $VCPKG_DIR/lib/libz.a ]; then
    ln -s $VCPKG_DIR/lib/libzlib.a $VCPKG_DIR/lib/libz.a
fi
# uc2_graphics & slideshow
if [ ! -f $VCPKG_DIR/lib/libfreeglut_static.a ]; then
    ln -s $VCPKG_DIR/lib/libfreeglut.a $VCPKG_DIR/lib/libfreeglut_static.a
fi

export CXXFLAGS="-I$VCPKG_DIR/include -L$VCPKG_DIR/lib"
export CFLAGS="$CXXFLAGS"

make
