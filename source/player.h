#ifndef PLAYER_H
#define PLAYER_H

#include <citro2d.h>
#include "globals.h"
#include "map.h"

typedef struct {
    float x, y;
    float dx, dy;
    bool canmove;
} Player;

void resetPlayer(Player* p, int spawnX, int spawnY);
void getPlayerTilePos(int* tileX, int* tileY, Player* p);
void getPlayerCornersXY(int corners[4][2], Player* p);
bool isWithinBounds(Player* p);
bool getCollision(Player* p, Tile map[15][25]);
void playerMovement(Player* p, Tile map[15][25], bool left, bool right, bool up);
void drawPlayer(Player* p);
void debugPlayerPosition(Player* p);

#endif // PLAYER_H