#ifndef MAP_H
#define MAP_H

#include <math.h>
#include <citro2d.h>
#include "globals.h"

typedef struct {
    int type;
    int rotation;
} Tile;

void drawTile(Tile t, int x, int y);
void generateMap(Tile map[15][25], int spawnX, int spawnY, int caveHeight, int wsChance, int blockVariety, int spawnBlock);
void loadRNGMap(Tile map[15][25], int seed, int iterations, int spawnX, int spawnY, int caveHeight, int wsChance, int blockVariety, int spawnBlock);
void drawMap(Tile map[15][25]);

// TODO: finish converting this to citro2d
// void mapSprite(C2D_Sprite sprite, Tile map[15][25]);

#endif // MAP_H