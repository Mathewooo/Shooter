#include "../common/common.h"
#include "bullets.h"
#include "movement.h"
#include "effects.h"

extern App app;
extern Core core;
extern Entity *player;

/* FIGHTER/BULLET MOVEMENT */

static bool entityCollision(Entity *target) {
    for (Entity *fighter = core.fighterHead.next; fighter != NULL; fighter = fighter->next)
        if (fighter->side != target->side && didCollide(
                target, fighter
        )) {
            target->health = 0; fighter->health = 0;
            addExplosions(fighter->x, fighter->y, 32), addDebris(fighter);
            return true;
        }
    return false;
}

void moveFighters(void) {
    Entity *fighter, *prev = &core.fighterHead;
    for (fighter = core.fighterHead.next; fighter != NULL; fighter = fighter->next) {
        fighter->x += fighter->dx, fighter->y += fighter->dy;
        if (fighter != player && fighter->x <= -fighter->w)
            fighter->health = 0;
        if (fighter->health == 0) {
            if (fighter == player) player = NULL;
            if (fighter == core.fighterTail) core.fighterTail = prev;
            prev->next = fighter->next;
            free(fighter);
            fighter = prev;
        }
        prev = fighter;
    }
}

void moveBullets(void) {
    Entity *bullet, *prev = &core.bulletHead;
    for (bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next) {
        bullet->x += bullet->dx, bullet->y += bullet->dy;
        if (entityCollision(bullet)
            || bullet->x <= -bullet->w
            || bullet->y <= -bullet->h
            || bullet->y >= SCREEN_HEIGHT
            || bullet->x >= SCREEN_WIDTH) {
            if (bullet == core.bulletTail)
                core.bulletTail = prev;
            prev->next = bullet->next;
            free(bullet);
            bullet = prev;
        }
        prev = bullet;
    }
}

/* PLAYER MOVEMENT */

static void clipPlayer(void) {
    if (player->x >= SCREEN_WIDTH / 2) player->dx -= PLAYER_SPEED;
    if (player->y + player->h >= SCREEN_HEIGHT) player->dy -= PLAYER_SPEED;
    if (player->x <= 0) player->dx += PLAYER_SPEED;
    if (player->y <= 0) player->dy += PLAYER_SPEED;
}

static void playerInput(void) {
    if (app.keyboard[SDL_SCANCODE_W]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_A]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_S]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_D]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->bulletReload <= 0) fireBullet();
    clipPlayer();
}

void movePlayer(void) {
    if (player != NULL) {
        player->dx = player->dy = 0;
        if (player->bulletReload > 0) player->bulletReload--;
        playerInput();
    }
}

/* DRAWING */
void drawFighters(void) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        renderTexture(entity->texture, entity->x, entity->y);
}