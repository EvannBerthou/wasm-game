#!/bin/sh

set -xe

CFLAGS='-Wall -Wextra -Werror -pedantic'
INCLUDES='-Isrc -isystem lib/include'
LIBS='-lm -pthread'
CC=${CC:-gcc}


emcc -o index.html $(find -type f -name "*.c") -Os -L./lib/web -l:libraylib.a $CFLAGS $INCLUDES -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB --preload-file img --preload-file data -FORCE_FILESYSTEM=1 -lidbfs.js -sASSERTIONS=2 -sPTHREAD_POOL_SIZE=8 -s USE_PTHREADS=1 -s INITIAL_MEMORY=512MB -s MAX_WEBGL_VERSION=2 -matomics -mbulk-memory

$CC -o game $(find -type f -name "*.c") $CFLAGS $INCLUDES -L./lib/linux -l:libraylib.a $LIBS -DPLATFORM_DESKTOP -g
