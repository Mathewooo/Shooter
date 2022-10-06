#include "common/common.h"
#include "textures/draw.h"
#include "init/init.h"
#include "input/input.h"
#include "core/core.h"

App app;
Core core;

static void capFrameRate(long *then, float *remainder);

//TODO Later on create a readme file describing all of util's functions and game's logic
//TODO Review defs.h to make gameplay more realistic
//TODO Make enemies also shoot straight as well as make them not shoot anymore if their x is lower than player's
//TODO Implement thrust animation for fighters

int main(void) {
    long then;
    float remainder;
    memset(&app, 0, sizeof(App));
    initSDL();
    atexit(cleanup);
    initCore();
    initRandomness();
    then = SDL_GetTicks();
    remainder = 0;
    while (true) {
        prepareScene();
        doInput();
        app.delegate.logic();
        app.delegate.draw();
        presentScene();
        capFrameRate(&then, &remainder);
    }
}

static void capFrameRate(long *then, float *remainder) {
    long wait, frameTime;
    wait = 16 + *remainder;
    *remainder -= (int) *remainder;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;
    if (wait < 1) wait = 1;
    SDL_Delay(wait);
    *remainder += 0.667;
    *then = SDL_GetTicks();
}
