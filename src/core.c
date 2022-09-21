#include "common/common.h"
#include "draw.h"
#include "core.h"

static void logic(void);

static void draw(void);

static void initPlayer(void);

static void fireBullet(void);

static void doPlayer(void);

static void doBullets(void);

static void drawPlayer(void);

static void drawBullets(void);

extern App app;
extern Core core;

static Entity *player;
static SDL_Texture *bulletTexture;

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    memset(&core, 0, sizeof(Core));
    core.fighterTail = &core.fighterHead;
    core.bulletTail = &core.bulletHead;
    initPlayer();
    bulletTexture = loadTexture("src/assets/playerBullet.png");
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
}

void clipPlayer(void) {
    if (player->x + player->w >= SCREEN_WIDTH) player->dx -= PLAYER_SPEED;
    if (player->y + player->h >= SCREEN_HEIGHT) player->dy -= PLAYER_SPEED;
    if (player->x <= 0) player->dx += PLAYER_SPEED;
    if (player->y <= 0) player->dy += PLAYER_SPEED;
}

void playerInput(void) {
    if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->bulletReload == 0) fireBullet();
    clipPlayer();
}

static void doPlayer(void) {
    player->dx = player->dy = 0;
    if (player->bulletReload > 0) player->bulletReload--;
    playerInput();
    player->x += player->dx;
    player->y += player->dy;
}

void updateBulletLoc(Entity *const bullet) {
    bullet->x = player->x + 40;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
}

void centerBulletTexture(Entity *const bullet) {
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
    bullet->y += (player->h / 2) - (bullet->h / 2);
}

static void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    updateBulletLoc(bullet);
    centerBulletTexture(bullet);
    player->bulletReload = BULLET_RELOAD;
}

static void doBullets(void) { //TRY TO UNDERSTAND THIS MORE!!!!: (SEE LINKED LISTS)
    Entity *bullet, *prev;
    prev = &core.bulletHead;
    for (bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next) {
        bullet->x += bullet->dx;
        bullet->y += bullet->dy;
        if (bullet->x >= SCREEN_WIDTH) {
            printf("Screen ended!\n"); //
            if (bullet == core.bulletTail) {
                printf("Bullet equals core.bulletTail!\n"); //
                core.bulletTail = prev;
            }
            prev->next = bullet->next;
            printf("Bullet reload at end of the screen: %d\n", bullet->bulletReload); //
            free(bullet);
            bullet = prev;
        }
        prev = bullet;
        printf("Prev Bullet reload: %d\n", prev->bulletReload); //
    }
}

static void logic(void) {
    doPlayer();
    doBullets();
}

static void draw(void) {
    drawPlayer();
    drawBullets();
}

static void drawPlayer(void) {
    blit(player->texture, player->x, player->y);
}

static void drawBullets(void) {
    for (Entity *bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next) {
        printf("x: %f\n", bullet->x); //
        blit(bullet->texture, bullet->x, bullet->y);
    }
}