#include <citro2d.h>
#include "globals.h"
#include "player.h"
#include "map.h"

void resetPlayer(Player* p, int spawnX, int spawnY) {
    p->x = (float)spawnX;
    p->y = (float)spawnY;
    p->dx = 0.0f;
    p->dy = 0.0f;
    p->canmove = true;

    printf("player reset to (%d, %d)\n", spawnX, spawnY);
}

void resetPlayerDefaults(Player* p) {
    p->jump = DEFAULT_JUMP;
    p->speed = DEFAULT_SPEED;
    p->gravity = DEFAULT_GRAVITY;
    p->friction = DEFAULT_FRICTION;
}

void getPlayerTilePos(int* tileX, int* tileY, Player* p) {
    *tileX = (int)(p->x) % TILE_SIZE;
    *tileY = (int)(p->y) % TILE_SIZE;
}

void getPlayerCornersXY(int corners[4][2], Player* p) {
    corners[0][0] = (int)(p->x);
    corners[0][1] = (int)(p->y); // Top-left

    corners[1][0] = (int)(p->x + PLAYER_SIZE);
    corners[1][1] = (int)(p->y); // Top-right

    corners[2][0] = (int)(p->x);
    corners[2][1] = (int)(p->y + PLAYER_SIZE); // Bottom-left

    corners[3][0] = (int)(p->x + PLAYER_SIZE);
    corners[3][1] = (int)(p->y + PLAYER_SIZE); // Bottom-right
}

bool isWithinBounds(Player* p) {
    int corners[4][2];
    getPlayerCornersXY(corners, p);

    for (int i = 0; i < 4; ++i) {
        int cx = corners[i][0];
        int cy = corners[i][1];
        if (cx < 0 || cx >= TOP_WIDTH) return false;
        if (cy < 0 || cy >= SCREEN_HEIGHT) return false;
    }
    return true;
}

bool getCollision(Player* p, MAP) {
    Tile buffer;
    int tl, tr, bl, br;

    if (getTileAtXY(&buffer, map, (int)p->x, (int)p->y) && p->y) tl = buffer.type;
    else tl = 0;
    if (getTileAtXY(&buffer, map, (int)p->x+PLAYER_SIZE-1, (int)p->y) && p->y) tr = buffer.type;
    else tr = 0;
    if (getTileAtXY(&buffer, map, (int)p->x, (int)p->y+PLAYER_SIZE-1) && p->y) bl = buffer.type;
    else bl = 0;
    if (getTileAtXY(&buffer, map, (int)p->x+PLAYER_SIZE-1, (int)p->y+PLAYER_SIZE-1) && p->y) br = buffer.type;
    else br = 0;

    return tl || tr || bl || br;
}

void playerMovement(Player* p, MAP, bool left, bool right, bool up) {
    if (!p->canmove) return;
    
    // handle vertical movement
    p->dy += p->gravity;
    p->y -= p->dy;
    if (getCollision(p, map) > 0 && p->y > 0) {
        // floor collision
        int safety = 0;
        while (getCollision(p, map) > 0 && safety < 50) {
            p->y += fabsf(p->dy) / p->dy;
            safety++;
        }

        // jump
        p->dy = p->jump * (up && fabsf(p->dy)/p->dy == -1);
    } else if (p->y < 0) {
        p->y = 0;
        p-> dy = 0;
    }

    // handle horizontal movement
    p->dx = p->friction * (p->dx + p->speed * (right - left));
    p->x += p->dx;
    if (getCollision(p, map) > 0 && p->x > 0) {
        // wall collision
        int safety = 0;
        while (getCollision(p, map) > 0 && safety < 20) {
            p->x -= fabsf(p->dx) / p->dx;
            safety++;
        }

        // wall jump
        if (up) {
            p->dy = p->jump;
            p->dx = fabsf(p->dx) / p->dx * -3;
        }
    } else if (p->x < 0) {
        p->x = 0;
        p->dx = 0;
    }
}

void drawPlayer(Player* p) {
    C2D_DrawRectSolid((int)p->x, (int)p->y, 0.0f, PLAYER_SIZE, PLAYER_SIZE, PLAYER_COLOR);
}

void debugPlayerPosition(Player* p) {
    printf("POS: %3f, %3f\n", p->x, p->y);
}