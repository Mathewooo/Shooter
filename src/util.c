#include "common/common.h"
#include "util.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

extern

void initRandomness(void) {
    srand(time(NULL));
}

long randomBound(long min, long max) {
    return min + (rand() % (max - min));
}

long randVal(long val) {
    return rand() % val;
}

int didCollide(const Entity *const e1, const Entity *const e2) {
    return (MAX(e1->x, e2->x) < MIN(e1->x + e1->w, e2->x + e2->w)) &&
           (MAX(e1->y, e2->y) < MIN(e1->y + e1->h, e2->y + e2->h));
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

    *dx = (x1 - x2);
    *dx /= steps;
    *dy = (y1 - y2);
    *dy /= steps;
}

void centerBulletTexture(Entity *const bullet,
                         const Entity *const entity) {
    SDL_QueryTexture(
            bullet->texture, NULL, NULL, &bullet->w, &bullet->h
    );
    bullet->x += (entity->w / 2) - (bullet->w / 2);
    bullet->y += (entity->h / 2) - (bullet->h / 2);
}