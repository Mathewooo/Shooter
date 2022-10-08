#include "../common/common.h"
#include "util.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void initRandomness(void) {
    srand(time(NULL));
}

long randBd(long min, long max) {
    return min + (rand() % (max - min));
}

long randVl(long val) {
    return rand() % val;
}

long randS(long val) {
    return rand() % val - rand() % val;
}

int didCollide(const Entity *const e,
               const Entity *const e2) {
    return (MAX(e->x, e2->x) < MIN(e->x + e->w, e2->x + e2->w)) &&
    (MAX(e->y, e2->y) < MIN(e->y + e->h, e2->y + e2->h));
}

void calcSlope(int x1, int y1,
               int x2, int y2,
               float *const dx, float *const dy) {
    int steps = MAX(abs(x1 - x2),
                    abs(y1 - y2));

    if (steps == 0) {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2), *dx /= steps;
    *dy = (y1 - y2), *dy /= steps;
}

void centerBulletTexture(Entity *const bullet,
                         const Entity *const entity) {
    SDL_QueryTexture(
            bullet->texture, NULL, NULL, &bullet->w, &bullet->h
    );
    bullet->x += (entity->w / 2) - (bullet->w / 2);
    bullet->y += (entity->h / 2) - (bullet->h / 2);
}

void setBulletFireType(Entity *const enemy,
                       const Entity *const player) {
    if (enemy->x < player->x) enemy->fireType = NONE;
    else {
        if ((enemy->y >= player->y - player->h
             && enemy->y <= player->y + player->h)
            && enemy->x > player->x)
            enemy->fireType = STRAIGHT;
        else if (enemy->x > player->x) enemy->fireType = MULTI_DIRECTIONAL;
    }
}