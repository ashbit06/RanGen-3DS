#include <3ds.h>
#include <citro2d.h>

#include "globals.h"
#include "player.h"
#include "map.h"

int spawnX = DEFAULT_SPAWNX;
int spawnY = DEFAULT_SPAWNY;
int spawnBlock = DEFAULT_SPAWN_BLOCK;
int caveHeight = DEFAULT_CAVE_HEIGHT;
int wsChance = DEFAULT_WS_CHANCE;
int blockVariety = DEFAULT_BLOCK_VARIETY;
int showTestTiles = DEFAULT_SHOW_TEST_TILES;
int allTimeCompleted = 0;

void draw(C3D_RenderTarget *top, Tile map[15][25], Player player) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BACKGROUND);
    C2D_SceneBegin(top);

    drawMap(map);
    drawPlayer(&player);

    C3D_FrameEnd(0);
}

int main()
{
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    cfguInit();
    consoleInit(GFX_BOTTOM, 0);

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    Player player;
    resetPlayer(&player, spawnX, spawnY);

    Tile map[15][25];
    generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START)
            break;
        if (kDown & KEY_L) {
            generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);
            resetPlayer(&player, spawnX, spawnY);
        }
        if (kDown & KEY_R || player.y > SCREEN_HEIGHT)
            resetPlayer(&player, spawnX, spawnY);
        if (kDown & KEY_X) {
            player.canmove = !player.canmove;
            consoleClear();
            printf("======= PAUSED =======\n");
            printf("Player Position: (%.2f, %.2f)\n", player.x, player.y);
            printf("Player Can Move: %s\n", player.canmove ? "true" : "false");
            int corners[4][2];
            getPlayerCornersXY(corners, &player);
            int cornerTiles[4][2];
            getPlayerTilePos(&cornerTiles[0][0], &cornerTiles[0][1], &player);
            getPlayerTilePos(&cornerTiles[1][0], &cornerTiles[1][1], &player);
            getPlayerTilePos(&cornerTiles[2][0], &cornerTiles[2][1], &player);
            getPlayerTilePos(&cornerTiles[3][0], &cornerTiles[3][1], &player);  
            printf("Player Corners:\n");
            printf("  Top-Left:     (%d, %d) | (%d, %d)\n", corners[0][0], corners[0][1], cornerTiles[0][0], cornerTiles[0][1]);
            printf("  Top-Right:    (%d, %d) | (%d, %d)\n", corners[1][0], corners[1][1], cornerTiles[1][0], cornerTiles[1][1]);
            printf("  Bottom-Left:  (%d, %d) | (%d, %d)\n", corners[2][0], corners[2][1], cornerTiles[2][0], cornerTiles[2][1]);
            printf("  Bottom-Right: (%d, %d) | (%d, %d)\n", corners[3][0], corners[3][1], cornerTiles[3][0], cornerTiles[3][1]);
            int collision = getCollision(&player, map);
            printf("Collision: %d\n", collision);
        }

        playerMovement(&player, map, kHeld & KEY_LEFT, kHeld & KEY_RIGHT, kHeld & KEY_UP);
        draw(top, map, player);
    }

    cfguExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
