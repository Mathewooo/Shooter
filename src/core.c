#include "common/common.h"
#include "core.h"

static void logic(void);

static void draw(void);

static void initPlayer(void);

static void fireBullet(void);

static void drawFighters(void);

static void drawBullets(void);

extern App app;
extern Core core;

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static int enemySpawnTimer;

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    memset(&core, 0, sizeof(Core));
    core.fighterTail = &core.fighterHead;
    core.bulletTail = &core.bulletHead;
    initPlayer();
    bulletTexture = loadTexture("src/assets/bullet.png");
    enemyTexture = loadTexture("src/assets/enemy.png");
    enemySpawnTimer = 0;
}

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    core.fighterTail->next = player;
    core.fighterTail = player;
    player->x = 100;
    player->y = SCREEN_HEIGHT / 2.0;
    player->texture = loadTexture("src/assets/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    player->side = SIDE_PLAYER;
}

static void clipPlayer(void) {
    if (player->x + player->w >= SCREEN_WIDTH)
        player->dx -= PLAYER_SPEED;
    if (player->y + player->h >= SCREEN_HEIGHT)
        player->dy -= PLAYER_SPEED;
    if (player->x <= 0) player->dx += PLAYER_SPEED;
    if (player->y <= 0) player->dy += PLAYER_SPEED;
}

static void playerInput(void) {
    if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->bulletReload == 0) fireBullet();
    clipPlayer();
}

static void movePlayer(void) {
    player->dx = player->dy = 0;
    if (player->bulletReload > 0) player->bulletReload--;
    playerInput();
}

static void initBullet(Entity *const bullet) {
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = player->x + 40;
    bullet->y = randomBound(player->y - BOUND, player->y + BOUND);
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    bullet->side = SIDE_PLAYER;
}

static void centerBulletTexture(Entity *const bullet) {
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
    bullet->y += (player->h / 2) - (bullet->h / 2);
}

static void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    initBullet(bullet);
    centerBulletTexture(bullet);
    player->bulletReload = BULLET_RELOAD;
}

static bool bulletHitFighter(Entity *bullet) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        if (entity->side != bullet->side && didCollide(bullet, entity)) {
            bullet->health = 0; entity->health = 0;
            return true;
        }
    return false;
}

static void moveBullets(void) {
    Entity *bullet, *prev = &core.bulletHead;
    for (bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next) {
        bullet->x += bullet->dx;
        if (bulletHitFighter(bullet) || bullet->x >= SCREEN_WIDTH) {
            if (bullet == core.bulletTail)
                core.bulletTail = prev;
            prev->next = bullet->next;
            free(bullet);
            bullet = prev;
        }
        prev = bullet;
    }
}

static void moveFighters(void) {
    Entity *fighter, *prev = &core.fighterHead;
    for (fighter = core.fighterHead.next; fighter != NULL; fighter = fighter->next) {
        fighter->x += fighter->dx;
        fighter->y += fighter->dy;
        if (fighter != player && (fighter->x < -fighter->w || fighter->health == 0)) {
            if (fighter == core.fighterTail)
                core.fighterTail = prev;
            prev->next = fighter->next;
            free(fighter);
            fighter = prev;
        }
        prev = fighter;
    }
}

static void initEnemy(Entity *const enemy) {
    core.fighterTail->next = enemy;
    core.fighterTail = enemy;
    enemy->x = SCREEN_WIDTH;
    enemy->y = randVal(SCREEN_HEIGHT);
    enemy->texture = enemyTexture;
    enemy->health = 1;
    enemy->side = SIDE_ALIEN;
}

static void spawnEnemies(void) {
    if (--enemySpawnTimer <= 0) {
        Entity *enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        initEnemy(enemy);
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        enemy->dx = randomBound(-5, 2);
        enemySpawnTimer = 35 + randVal(70);
    }
}

static void logic(void) {
    movePlayer();
    moveFighters();
    moveBullets();
    spawnEnemies();
}

static void draw(void) {
    drawFighters();
    drawBullets();
}

static void drawFighters(void) {
    for (Entity *entity = core.fighterHead.next; entity != NULL; entity = entity->next)
        blit(entity->texture, entity->x, entity->y);
}

static void drawBullets(void) {
    for (Entity *bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next)
        blit(bullet->texture, bullet->x, bullet->y);
}