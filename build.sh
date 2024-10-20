#!/bin/sh

set -xe

CFLAGS='-Wall -Wextra -Werror'
INCLUDES='-Isrc -isystem lib/include'
LIBS='-lm'
CC=${CC:-gcc}


#emcc -o index.html $(find -type f -name "*.c") -Os -L./lib/web -l:libraylib.a $CFLAGS $INCLUDES -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB --preload-file img --preload-file data -FORCE_FILESYSTEM=1 -lidbfs.js

$CC -o game $(find -type f -name "*.c") $CFLAGS $INCLUDES -L./lib/linux -l:libraylib.a $LIBS -DPLATFORM_DESKTOP -g
