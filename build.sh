#!/bin/sh

set -xe

emcc -o index.html src/*.c -Os -Wall libraylib.a -L./libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB
cc -o game src/*.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP -g
