#!/bin/sh

set -xe

emcc -o index.html $(find -type f -name "*.c") -Os -Wall libraylib.a -I../raylib/src -L./libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB --preload-file img --preload-file data
cc -o game $(find -type f -name "*.c") -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP -g
