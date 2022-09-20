#include "common/common.h"
#include "input.h"

static void doKeyUp(SDL_KeyboardEvent *event);
static void doKeyDown(SDL_KeyboardEvent *event);

extern App app;

const Keys keysArray[5] = {
        {SDL_SCANCODE_UP, &app.up},
        {SDL_SCANCODE_DOWN, &app.down},
        {SDL_SCANCODE_LEFT, &app.left},
        {SDL_SCANCODE_RIGHT, &app.right},
        {SDL_SCANCODE_SPACE, &app.fire}
};

const size_t keysArrSize = sizeof keysArray / sizeof *keysArray;

void doInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
            default:
                break;
        }
    }
}

void keyAction(SDL_KeyboardEvent *event, int value) {
    for (int index = 0; index < keysArrSize; index++) {
        const Keys current = keysArray[index];
        if (event->keysym.scancode == current.scanCode) {
            *current.direction = value;
            break;
        }
    }
}

static void doKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) keyAction(event, 0);
}

static void doKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) keyAction(event, 1);
}
