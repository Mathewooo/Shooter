#include "common/common.h"
#include "util.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void initRandomness(void) {
    srand(time(NULL));
}

long randomBound(long min, long max) {
    return min + (rand() % (max - min));
}

long randVal(long val) {
    return rand() % val;
}

int didCollide(const Entity *const e1, const Entity *const e2) { //UNDERSTAND THIS MORE!!
    return (MAX(e1->x, e2->x) < MIN(e1->x + e1->w, e2->x + e2->w))
    && (MAX(e1->y, e2->y) < MIN(e1->y + e1->h, e2->y + e2->h));
}