#!/bin/bash

clear

clang \
-Oz -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
-Wformat -Wformat-security -Werror=format-security \
./src/*.c -lsdl2 -lsdl2_image -lsdl2_mixer -lsdl2_ttf \
-lm -o Shooter

./Shooter