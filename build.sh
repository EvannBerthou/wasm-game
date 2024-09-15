#!/bin/sh

set -xe

emcc -o index.html main.c -Os -Wall libraylib.a -I. -L. -L./libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB
cc -o game main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
