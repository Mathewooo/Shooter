#include "common/common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

#define PLAYER_SPEED 5
#define WINDOW_OFFSET 48
#define WINDOW_X (SCREEN_WIDTH - WINDOW_OFFSET)
#define WINDOW_Y (SCREEN_HEIGHT - WINDOW_OFFSET)

App app;
Entity player;
Entity bullet;

void initPlayer(void);

void playerMovement(void);

void initBullet(void);

void fireBullet(void);

int main(void) {
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
    memset(&bullet, 0, sizeof(Entity));

    initSDL();
    initPlayer();
    initBullet();
    atexit(cleanup);

    while (1) {
        prepareScene();
        doInput();
        playerMovement();
        fireBullet();
        presentScene();
        SDL_Delay(16);
    }
}

void fireBullet(void) {
    if (app.fire && bullet.health == 0) {
        bullet.x = player.x + 50;
        bullet.y = player.y + 18;
        bullet.dx = 15;
        bullet.dy = 0;
        bullet.health = 1;
    }

    bullet.x += bullet.dx;
    bullet.y += bullet.dy;

    if (bullet.x > SCREEN_WIDTH) bullet.health = 0;
    blit(player.texture, player.x, player.y);
    if (bullet.health > 0) blit(bullet.texture, bullet.x, bullet.y);
}

void initPlayer(void) {
    player.x = 150;
    player.y = 200;
    player.texture = loadTexture("src/assets/player.png");
}

void initBullet(void) {
    bullet.texture = loadTexture("src/assets/playerBullet.png");
}

void boundsCheck(void) {
    if (player.x >= WINDOW_X) player.x = WINDOW_X;
    if (player.y >= WINDOW_Y) player.y = WINDOW_Y;
    if (player.x <= 0) player.x = 0;
    if (player.y <= 0) player.y = 0;
}

void playerMovement(void) {
    if (app.up) player.y -= PLAYER_SPEED;
    if (app.down) player.y += PLAYER_SPEED;
    if (app.left) player.x -= PLAYER_SPEED;
    if (app.right) player.x += PLAYER_SPEED;
    boundsCheck();
}