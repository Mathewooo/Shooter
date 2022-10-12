#include "../common/common.h"
#include "../textures/draw.h"
#include "bullets.h"
#include "effects.h"
#include "movement.h"
#include "core.h"

static void logic(void);
static void draw(void);
static void resetCore(void);
static void initPlayer(void);

extern App app;
extern Core core;

Entity *player;
SDL_Texture *playerTexture, *bulletTexture,
        *enemyTexture, *enemyBulletTexture,
        *background, *explosionTexture;
static int enemySpawnTimer, coreResetTimer;

void cacheTextures(void) {
    playerTexture = loadTexture(
            "src/assets/player.png"
            ),
    bulletTexture = loadTexture(
            "src/assets/bullet.png"
            ),
    enemyTexture = loadTexture(
            "src/assets/enemy.png"
            ),
    enemyBulletTexture = loadTexture(
            "src/assets/enemyBullet.png"
            ),
    background = loadTexture(
            "src/assets/background.png"
            ),
    explosionTexture = loadTexture(
            "src/assets/explosion.png"
            );
}

void initCore(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    cacheTextures();
    resetCore();
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
    core.explosionTail = &core.explosionHead;
    core.debrisTail = &core.debrisHead;
    initPlayer();
    initEffects();
    enemySpawnTimer = 0, coreResetTimer = FPS * 2;
}

/* INITIALIZERS (FIGHTERS) */

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    core.fighterTail->next = player, core.fighterTail = player;
    player->health = 1, player->x = 100, player->texture = playerTexture;
    SDL_QueryTexture(
            player->texture, NULL, NULL, &player->w, &player->h
    );
    player->y = (SCREEN_HEIGHT / 2.0) - player->h, player->side = SIDE_PLAYER;
}

static void initEnemy(Entity *const enemy) {
    core.fighterTail->next = enemy, core.fighterTail = enemy;
    enemy->x = SCREEN_WIDTH,
    enemy->y = randVl(
            SCREEN_HEIGHT - enemy->h
    );
    enemy->dx = randBd(
            -4, 1
    );
    enemy->health = 1, enemy->side = SIDE_ALIEN;
    enemy->bulletReload = FPS * randBd(
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
        enemySpawnTimer = randBd(25, 75);
    }
}

/* LOGIC METHOD */

static void logic(void) {
    moveStarfield();
    movePlayer();
    attackingEnemies();
    moveFighters();
    moveBullets();
    moveExplosions();
    moveDebris();
    spawnEnemies();
    if (player == NULL && --coreResetTimer <= 0) resetCore();
}

/* DRAW METHOD */

static void draw(void) {
    drawBackground();
    drawStarfield();
    drawFighters();
    drawDebris();
    drawExplosions();
    drawBullets();
}
