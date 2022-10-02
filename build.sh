#!/bin/bash

clear

clang \
-g -Wall \
./src/*.c -lsdl2 -lsdl2_image -lsdl2_mixer -lsdl2_ttf \
-lm -o Shooter

./Shooter