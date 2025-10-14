#include <math.h>
#include <citro2d.h>
#include "globals.h"
#include "map.h"

void drawTile(Tile t, int x, int y) {
    if (t.type == 1)
        C2D_DrawRectSolid(x, y, 0.0f, TILE_SIZE, TILE_SIZE, BLACK);
    // else draw nothing for empty tile
}

bool getTileAtXY(Tile* tile, MAP, int x, int y) {
    if (!tile) return false;
    if (x < 0 || x >= TOP_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return false;
    *tile = map[y / TILE_SIZE][x / TILE_SIZE];
    return true;
}

bool getMapPosXY(int x, int y, int r, int c) {
    if (r < 0 || r > 15 || c < 0 || c > 25) return false;
    x = c * TILE_SIZE;
    y = r * TILE_SIZE;
    return true;
}

void generateMap(MAP, int spawnX, int spawnY, int caveHeight, int wsChance, int blockVariety, int spawnBlock) {
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 25; x++) {
            map[y][x].rotation = 0;
            if (((rand() % ((int)log(y*TILE_SIZE) * (SCREEN_HEIGHT/2) + (SCREEN_HEIGHT/2) + abs(caveHeight-500))) + 1) > (wsChance * 6) || ((x*TILE_SIZE == spawnX) && (y*TILE_SIZE == spawnY - TILE_SIZE))) {
                map[y][x].type = 1;
            } else {
                map[y][x].type = 0;  // Empty tile
            }

            // printf("coords: (%d, %d) \ttype: %d \trotation: %d\n", x, y, map[y][x].type, map[y][x].rotation);
        }
    }
    printf("new map generated\n");
}

void drawMap(MAP) {
    for (int y = 0; y < 18; y++) {
        for (int x = 0; x < 25; x++) {
            drawTile(map[y][x], x * TILE_SIZE, y * TILE_SIZE);
        }
    }
}

// TODO: finish converting this to citro2d
// void mapSprite(C2D_Sprite sprite, MAP) {
//     // uint8_t width = 16, height = 16;
//     // size_t size = sizeof(gfx_sprite_t) + width * height * sizeof(uint8_t);
//     // gfx_sprite_t *sprite = malloc(size);

//     // sprite->width = width;
//     // sprite->height = height;

//     // // draw the map onto the sprite data
    
//     drawMap(map, 0);
//     for (int i = 0; i < sprite.params.pos.w * sprite.params.pos.h; i++) {
//         if (sprite.data[i] == BG_COLOR) {
//             sprite.data[i] = 0xFF; // set background color to transparent
//         }
//     }

//     for (int j = 0; j < sprite.params.pos.h; j++) {
//         for (int i = 0; i < sprite.params.pos.w; i++) {
//             sprite.data[j * sprite.params.pos.w + i] = gfx_GetPixel(i, j);
//         }
//     }

// }
