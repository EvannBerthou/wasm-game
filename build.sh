#!/bin/sh

set -xe

emcc -o index.html $(find -type f -name "*.c") -Os -Wall libraylib.a -I../raylib/src -Isrc -L./libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB --preload-file img --preload-file data
cc -o game $(find -type f -name "*.c") -Wall -Wextra -Werror -Isrc -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP -g
