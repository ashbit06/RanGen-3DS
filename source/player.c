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

// 0 for no collision, 1 for block collision, 2 for death collision
int getCollision(Player* p, Tile map[15][25]) {
    if (!isWithinBounds(p)) {
        printf("isWithinBounds: player is out of bounds\n");
        return 0;
    }
    // int collision = 0;

    // get the corners of the player and get which tiles they reside in, then check if they are within a block on each tile
    Tile t;
    int corners[4][2];
    getPlayerCornersXY(corners, p);
    int cornerTileX, cornerTileY;
    
    int collisions[4] = {0, 0, 0, 0};
    for (int c = 0; c < 4; c++) {
        if (!getTileAtXY(t, map, corners[c][0], corners[c][1])) {
            printf("getTileAtXY: tile not found\n");
            continue;
        }
        cornerTileX = corners[c][0] % TILE_SIZE;
        cornerTileY = corners[c][1] % TILE_SIZE;
        if (t.type == 0) continue; // empty tile

        // check if the corner is within the block
        if (t.type == 1) collisions[c] =
            cornerTileX >= 0 && cornerTileX < TILE_SIZE &&
            cornerTileY >= 0 && cornerTileY < TILE_SIZE ? 1 : 0; // block
    }

    return max(collisions, 4);
}

void playerMovement(Player* p, Tile map[15][25], bool left, bool right, bool up) {
    if (!p->canmove) return;
    
    // handle vertical movement
    p->dy += GRAVITY;
    p->y -= p->dy;
    if (getCollision(p, map)) {
        // floor collision
        int safety = 0;
        while (getCollision(p, map) && safety < 50) {
            p->y += fabsf(p->dy) / p->dy;
            safety++;
        }

        // jump
        p->dy = -JUMP * (up && fabsf(p->dy)/p->dy == -1);
    }

    // handle horizontal movement
    p->dx = FRICTION * (p->dx + SPEED*right - SPEED*left*2);
    p->x += p->dx;
    if (getCollision(p, map)) {
        // wall collision
        int safety = 0;
        while (getCollision(p, map) && safety < 20) {
            p->x -= fabsf(p->dx) / p->dx;
            safety++;
        }

        // wall jump
        if (up) {
            p->dy = -JUMP;
            p->dx = fabsf(p->dx) / p->dx * -6;
        }
    }
}

void drawPlayer(Player* p) {
    C2D_DrawRectSolid((int)(p->x - PLAYER_SIZE / 2), (int)(p->y - PLAYER_SIZE / 2), 0.0f, PLAYER_SIZE, PLAYER_SIZE, PLAYER_COLOR);
}

void debugPlayerPosition(Player* p) {
    printf("POS: %3f, %3f\n", p->x, p->y);
}