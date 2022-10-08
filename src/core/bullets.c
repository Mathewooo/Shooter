#include "../common/common.h"
#include "core.h"
#include "bullets.h"

extern Core core;
extern Entity *player;

/* BULLET FIRING */

static void initBullet(Entity *const bullet) {
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = player->x + 40, bullet->y = randBd(
            player->y - BOUND, player->y + BOUND
    ), bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1, bullet->texture = bulletTexture, bullet->side = SIDE_PLAYER;
}

void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    initBullet(bullet);
    centerBulletTexture(bullet, player);
    player->bulletReload = BULLET_RELOAD;
}

static void initEnemyBullet(const Entity *const entity, Entity *const bullet) {
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = entity->x, bullet->y = entity->y, bullet->health = 1;
    bullet->texture = enemyBulletTexture, bullet->side = SIDE_ALIEN;
}

void fireEnemyBullet(Entity *entity) {
    if (entity->fireType == NONE) return;
    Entity *bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    initEnemyBullet(entity, bullet);
    centerBulletTexture(bullet, entity);
    if (entity->fireType == MULTI_DIRECTIONAL) {
        calcSlope(
                player->x + (player->w / 2), player->y + (player->h / 2),
                entity->x, entity->y,
                &bullet->dx, &bullet->dy
        );
        bullet->dx *= ALIEN_BULLET_SPEED, bullet->dy *= ALIEN_BULLET_SPEED;
    } else if (entity->fireType == STRAIGHT)
        bullet->dx = -ALIEN_BULLET_SPEED;
    entity->bulletReload = randBd(
            FPS * 2, FPS * 2.5
    );
}

void attackingEnemies(void) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        if (entity != player && player != NULL && --entity->bulletReload <= 0) {
            setBulletFireType(entity, player);
            fireEnemyBullet(entity);
        }
}

/* DRAWING */

void drawBullets(void) {
    for (Entity *bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next)
        renderTexture(bullet->texture, bullet->x, bullet->y);
}