#!/bin/bash

clear

gcc \
-g -Wall \
-std=c17 \
./src/*.c -lsdl2 -lsdl2_image -lsdl2_mixer -lsdl2_ttf \
-lm -o Shooter

./Shooter