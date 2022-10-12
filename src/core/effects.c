#include "../common/common.h"
#include "core.h"
#include "effects.h"

extern App app;
extern Core core;

static Star stars[MAX_STARS];
static SDL_Rect bgDest;

/* INIT */

void initStarfield(void) {
    for (int i = 0; i < MAX_STARS; i++)
        stars[i].x = randVl(SCREEN_WIDTH),
        stars[i].y = randVl(SCREEN_HEIGHT),
        stars[i].speed = randBd(2, 5);
}

void initBgDest(void) {
    bgDest.x = 0, bgDest.y = 0,
    bgDest.w = SCREEN_WIDTH, bgDest.h = SCREEN_HEIGHT;
}

void initEffects(void) {
    initStarfield();
    initBgDest();
}

/* MOVING */

void moveExplosions(void) {
    Explosion *explosion, *prev = &core.explosionHead;
    for (explosion = core.explosionHead.next; explosion != NULL; explosion = explosion->next) {
        explosion->x += explosion->dx, explosion->y += explosion->dy;
        if (--explosion->a <= 0) {
            if (explosion == core.explosionTail)
                core.explosionTail = prev;
            prev->next = explosion->next;
            free(explosion);
            explosion = prev;
        }
        prev = explosion;
    }
}

void moveDebris(void) {
    Debris *debris, *prev = &core.debrisHead;
    for (debris = core.debrisHead.next; debris != NULL; debris = debris->next) {
        debris->x += debris->dx, debris->y += debris->dy,
        debris->dy += 0.4;
        if (--debris->life <= 0) {
            if (debris == core.debrisTail)
                core.debrisTail = prev;
            prev->next = debris->next;
            free(debris);
            debris = prev;
        }
        prev = debris;
    }
}

/* STARFIELD MOVEMENT */

void moveStarfield(void) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x -= stars[i].speed;
        if (stars[i].x < 0)
            stars[i].x = SCREEN_WIDTH + stars[i].x;
    }
}

/* ADDING */

void addExplosions(int x, int y, int num) {
    Explosion *explosion;
    for (int i = 0; i < num; i++) {
        explosion = malloc(sizeof(Explosion));
        memset(explosion, 0, sizeof(Explosion));
        core.explosionTail->next = explosion, core.explosionTail = explosion;
        explosion->x = x + randS(32), explosion->y = y + randS(32);
        explosion->dx = randS(10), explosion->dy = randS(10);
        explosion->dx /= 10, explosion->dy /= 10;

        switch (randVl(4)) {
            case 0:
                explosion->r = 255;
                break;
            case 1:
                explosion->r = 255;
                explosion->g = 128;
                break;
            case 2:
                explosion->r = 255;
                explosion->g = 255;
                break;
            default:
                explosion->r = 255;
                explosion->g = 255;
                explosion->b = 255;
                break;
        }

        explosion->a = randVl(FPS) * 3;
    }
}

void addDebris(Entity *fighter) {
    Debris *debris;
    int x, y, w, h;
    w = fighter->w / 2, h = fighter->h / 2;
    for (y = 0; y <= h; y += h)
        for (x = 0; x <= w; x += w)
        {
            debris = malloc(sizeof(Debris));
            memset(debris, 0, sizeof(Debris));
            core.debrisTail->next = debris, core.debrisTail = debris;
            debris->x = fighter->x + fighter->w / 2, debris->y = fighter->y + fighter->h / 2;
            debris->dx = randS(5), debris->dy = randBd(
                    -10, -5
            );
            debris->life = FPS * 2, debris->texture = fighter->texture;
            debris->rect.x = x, debris->rect.y = y,
            debris->rect.w = w, debris->rect.h = h;
        }
}

/* DRAWING */

void drawBackground(void) {
    SDL_RenderCopy(
            app.renderer, background, NULL, &bgDest
            );
}

void drawStarfield(void) {
    int i, c;
    for (i = 0; i < MAX_STARS; i++) {
        c = 32 * stars[i].speed;
        SDL_SetRenderDrawColor(
                app.renderer, c, c, c, 255
        );
        SDL_RenderDrawLine(
                app.renderer,
                stars[i].x, stars[i].y,
                stars[i].x + 3, stars[i].y
        );
    }
}

void drawDebris(void) {
    for (Debris *debris = core.debrisHead.next ; debris != NULL ; debris = debris->next)
        renderRect(
                debris->texture, &debris->rect, debris->x, debris->y
        );
}

void drawExplosions(void) {
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);
    for (Explosion *explosion = core.explosionHead.next; explosion != NULL; explosion = explosion->next) {
        SDL_SetTextureColorMod(
                explosionTexture,
                explosion->r, explosion->g, explosion->b
        );
        SDL_SetTextureAlphaMod(
                explosionTexture, explosion->a
        );
        renderTexture(explosionTexture, explosion->x, explosion->y);
    }
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}