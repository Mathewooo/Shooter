#include "common/common.h"
#include "draw.h"
#include "core.h"

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
    player->y = 100;
    player->texture = loadTexture("src/assets/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void doPlayer(void) {
    player->dx = player->dy = 0;

    if (player->reload > 0)
        player->reload--;

    if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0) fireBullet();

    player->x += player->dx;
    player->y += player->dy;
}

static void fireBullet(void) {
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    core.bulletTail->next = bullet;
    core.bulletTail = bullet;
    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;

    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);
    player->reload = 8;
}

static void doBullets(void) {
    Entity *b, *prev;
    prev = &core.bulletHead;
    for (b = core.bulletHead.next; b != NULL; b = b->next) {
        b->x += b->dx;
        b->y += b->dy;
        if (b->x > SCREEN_WIDTH) {
            if (b == core.bulletTail)
                core.bulletTail = prev;
            prev->next = b->next;
            free(b);
            b = prev;
        }
        prev = b;
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
    Entity *bullet;
    for (bullet = core.bulletHead.next; bullet != NULL; bullet = bullet->next)
        blit(bullet->texture, bullet->x, bullet->y);
}