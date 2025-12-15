#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define TOP_WIDTH      400
#define BOTTOM_WIDTH   320
#define SCREEN_HEIGHT  240

#define WHITE       C2D_Color32(255, 255, 255, 255)
#define BLACK       C2D_Color32(0, 0, 0, 255)
#define RED         C2D_Color32(255, 0, 0, 255)
#define BACKGROUND  C2D_Color32(210, 210, 210, 255)
#define PLAYER_COLOR C2D_Color32(255, 50, 50, 255)

#define TILE_SIZE  16
#define BG_COLOR    1

#define DEFAULT_SPAWNX (int)(TILE_SIZE/2)
#define DEFAULT_SPAWNY (int)(SCREEN_HEIGHT/2) - 1
#define DEFAULT_SPAWN_BLOCK     1
#define DEFAULT_CAVE_HEIGHT     0
#define DEFAULT_WS_CHANCE      99
#define DEFAULT_BLOCK_VARIETY  75
#define DEFAULT_SHOW_TEST_TILES 0

typedef struct {
    int x, y;
} PosI;

typedef struct {
    float x, y;
} PosF;

struct settings {
    char spawnX[4];
    char spawnY[4];
    char spawnBlock[4];
    char caveHeight[4];
    char wsChance[4];
    char blockVariety[4];
    char showTestTiles[2];
    char allTimeCompleted[16];
};

extern struct settings storedSettings;

int max(const int *arr, size_t n);
int max(const int *arr, size_t n);
bool any(bool array[], int size);
bool all(bool array[], int size);
bool startsWith(const char* str, const char* prefix);

#endif // DEFINES_H