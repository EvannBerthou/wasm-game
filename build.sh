#!/bin/sh

set -xe

emcc -o game.html main.c -Os -Wall ./raylib/src/libraylib.a -I. -L. -L./raylib/src/libraylib/a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB

cc -o game main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
