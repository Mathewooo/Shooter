#include "common/common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "core.h"

App app;
Core core;

static void capFrameRate(long *then, float *remainder);

//TODO Later on create a readme file describing all of util's functions and game's logic

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
