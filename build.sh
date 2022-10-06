#!/bin/bash

clear

clang \
  -Oz -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
  -Wformat -Wformat-security -Werror=format-security \
  ./src/*.c \
  ./src/core/*.c \
  ./src/init/*.c \
  ./src/textures/*.c \
  ./src/util/*.c \
  ./src/input/*.c \
  -lsdl2 -lsdl2_image \
  -lm -o Shooter

./Shooter
