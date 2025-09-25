#include <3ds.h>
#include <citro2d.h>

#include "globals.h"
#include "map.h"

int spawnX = DEFAULT_SPAWNX;
int spawnY = DEFAULT_SPAWNY;
int spawnBlock = DEFAULT_SPAWN_BLOCK;
int caveHeight = DEFAULT_CAVE_HEIGHT;
int wsChance = DEFAULT_WS_CHANCE;
int blockVariety = DEFAULT_BLOCK_VARIETY;
int showTestTiles = DEFAULT_SHOW_TEST_TILES;
int allTimeCompleted = 0;

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

    Tile map[15][25];
    generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (K_START)
            break;
        if (K_L)
            generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);
        // if (K_R)
        //     resetPlayer();

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, BACKGROUND);
        C2D_SceneBegin(top);
        
        drawMap(map);

        C3D_FrameEnd(0);
    }

    cfguExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
