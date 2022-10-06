#include "common/common.h"
#include "core.h"
#include "draw.h"

static void logic(void);

static void draw(void);

static void fireBullet(void);

static void resetCore(void);

extern App app;
extern Core core;

static Entity *player;
static SDL_Texture *playerTexture;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *enemyBulletTexture;
static int enemySpawnTimer;
static int coreResetTimer;

void cacheTextures(void) {
    playerTexture = loadTexture(
            "src/assets/player.png"
    );
    bulletTexture = loadTexture(
            "src/assets/bullet.png"
    );
    enemyTexture = loadTexture(
            "src/assets/enemy.png"
    );
    enemyBulletTexture = loadTexture(
            "src/assets/enemyBullet.png"
    );
}

void initCore(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    cacheTextures();
    resetCore();
}

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    core.fighterTail->next = player;
    core.fighterTail = player;
    player->health = 1;
    player->x = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(
            player->texture, NULL, NULL, &player->w, &player->h
    );
    player->y = (SCREEN_HEIGHT / 2.0) - player->h;
    player->side = SIDE_PLAYER;
}

static void resetCore(void) {
    Entity *entity;

    while (core.fighterHead.next) {
        entity = core.fighterHead.next;
        core.fighterHead.next = entity->next;
        free(entity);
    }

    while (core.bulletHead.next) {
        entity = core.bulletHead.next;
        core.bulletHead.next = entity->next;
        free(entity);
    }

    memset(&core, 0, sizeof(Core));
    core.fighterTail = &core.fighterHead;
    core.bulletTail = &core.bulletHead;
    initPlayer();
    enemySpawnTimer = 0;
    coreResetTimer = FPS * 2;
}

static void clipPlayer(void) {
    if (player->x >= SCREEN_WIDTH / 2) player->dx -= 3.5;
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

static void movePlayer(void) {
    if (player != NULL) {
        player->dx = player->dy = 0;
        if (player->bulletReload > 0) player->bulletReload--;
        playerInput();
    }
}

static void initBullet(Entity *const bullet) {
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = player->x + 40;
    bullet->y = randBound(player->y - BOUND, player->y + BOUND);
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    bullet->side = SIDE_PLAYER;
}

static void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    initBullet(bullet);
    centerBulletTexture(bullet, player);
    player->bulletReload = BULLET_RELOAD;
}

static int entityCollision(Entity *target) {
    for (Entity *fighter = core.fighterHead.next; fighter != NULL; fighter = fighter->next)
        if (fighter->side != target->side && didCollide(
                target, fighter
        )) {
            target->health = 0; fighter->health = 0;
            return 1;
        }
    return 0;
}

static void moveFighters(void) {
    Entity *fighter, *prev = &core.fighterHead;
    for (fighter = core.fighterHead.next; fighter != NULL; fighter = fighter->next) {
        fighter->x += fighter->dx;
        fighter->y += fighter->dy;
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

static void moveBullets(void) {
    Entity *bullet, *prev = &core.bulletHead;
    for (bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next) {
        bullet->x += bullet->dx;
        bullet->y += bullet->dy;
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

static void initEnemy(Entity *const enemy) {
    core.fighterTail->next = enemy;
    core.fighterTail = enemy;
    enemy->x = SCREEN_WIDTH;
    enemy->y = randVal(SCREEN_HEIGHT - enemy->h);
    enemy->dx = randBound(-4, 3);
    enemy->health = 1;
    enemy->side = SIDE_ALIEN;
    enemy->bulletReload = FPS * randBound(
            2, 3
    );
}

static void spawnEnemies(void) {
    if (--enemySpawnTimer <= 0) {
        Entity *enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        enemy->texture = enemyTexture;
        SDL_QueryTexture(
                enemy->texture, NULL, NULL, &enemy->w, &enemy->h
        );
        initEnemy(enemy);
        enemySpawnTimer = 35 + randVal(70);
    }
}

static void initEnemyBullet(const Entity *const entity, Entity *const bullet) {
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = entity->x;
    bullet->y = entity->y;
    bullet->health = 1;
    bullet->texture = enemyBulletTexture;
    bullet->side = SIDE_ALIEN;
}

static void fireEnemyBullet(Entity *entity) {
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
        bullet->dx *= ALIEN_BULLET_SPEED;
        bullet->dy *= ALIEN_BULLET_SPEED;
    } else if (entity->fireType == STRAIGHT)
        bullet->dx = -ALIEN_BULLET_SPEED;
    entity->bulletReload = randBound(
            FPS * 2, FPS * 2.5
    );
}

static void attackingEnemies(void) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        if (entity != player && player != NULL && --entity->bulletReload <= 0) {
            setBulletFireType(entity, player);
            fireEnemyBullet(entity);
        }
}

static void logic(void) {
    movePlayer();
    attackingEnemies();
    moveFighters();
    moveBullets();
    spawnEnemies();
    if (player == NULL && --coreResetTimer <= 0) resetCore();
}

static void drawFighters(void) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        blit(entity->texture, entity->x, entity->y);
}

static void drawBullets(void) {
    for (Entity *bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next)
        blit(bullet->texture, bullet->x, bullet->y);
}

static void draw(void) {
    drawFighters();
    drawBullets();
}
