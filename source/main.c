#include <3ds.h>
#include <citro2d.h>
#include <time.h>

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

void draw(C3D_RenderTarget *top, MAP, Player player) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BACKGROUND);
    C2D_SceneBegin(top);

    drawMap(map);
    drawPlayer(&player);

    C3D_FrameEnd(0);
}

bool playerAttributeAdjustment(Player* p, u32 kDown, u32 kHeld, MAP) {
    if (p->canmove) return false;
    if (kDown & KEY_Y) {
        resetPlayerDefaults(p);
        return true;
    }

    int8_t selected = -1;
    float buffer = 0.0f;

    if (kHeld & KEY_L)  { selected = 0; buffer = p->speed; }
    if (kHeld & KEY_ZL) { selected = 1; buffer = p->friction; }
    if (kHeld & KEY_ZR)  { selected = 2; buffer = p->jump; }
    if (kHeld & KEY_R) { selected = 3; buffer = p->gravity; }
    if (selected < 0) return false;
    
    if (kDown & KEY_DDOWN) buffer -= 0.01f;
    else if (kDown & KEY_DUP) buffer += 0.01f;
    else return false;

    switch (selected) {
        case 0:
        p->speed = buffer; break;
        case 1:
        p->friction = buffer; break;
        case 2:
        p->jump = buffer; break;
        case 3:
        p->gravity = buffer; break;
        default:
        return false;
    }

    return true;
}

void printPauseScreen(Player p, MAP) {
    consoleClear();
    printf("======= PAUSED =======\n\n");
    printf("Player Position: (%.2f, %.2f)\n", p.x, p.y);
    printf("Player Can Move: %s\n", p.canmove ? "true" : "false");
    int corners[4][2];
    getPlayerCornersXY(corners, &p);
    int cornerTiles[4][2];
    getPlayerTilePos(&cornerTiles[0][0], &cornerTiles[0][1], &p);
    getPlayerTilePos(&cornerTiles[1][0], &cornerTiles[1][1], &p);
    getPlayerTilePos(&cornerTiles[2][0], &cornerTiles[2][1], &p);
    getPlayerTilePos(&cornerTiles[3][0], &cornerTiles[3][1], &p);  
    printf("Player Corners:\n");
    printf("  Top-Left:     (%d, %d) | (%d, %d)\n", corners[0][0], corners[0][1], cornerTiles[0][0], cornerTiles[0][1]);
    printf("  Top-Right:    (%d, %d) | (%d, %d)\n", corners[1][0], corners[1][1], cornerTiles[1][0], cornerTiles[1][1]);
    printf("  Bottom-Left:  (%d, %d) | (%d, %d)\n", corners[2][0], corners[2][1], cornerTiles[2][0], cornerTiles[2][1]);
    printf("  Bottom-Right: (%d, %d) | (%d, %d)\n", corners[3][0], corners[3][1], cornerTiles[3][0], cornerTiles[3][1]);
    int collision = getCollision(&p, map);
    printf("Collision: %d\n", collision);
    Tile tl, tr, bl, br;
    getTileAtXY(&tl, map, (int)p.x, (int)p.y);
    getTileAtXY(&tr, map, (int)p.x + PLAYER_SIZE - 1, (int)p.y);
    getTileAtXY(&bl, map, (int)p.x, (int)p.y + PLAYER_SIZE - 1);
    getTileAtXY(&br, map, (int)p.x + PLAYER_SIZE - 1, (int)p.y + PLAYER_SIZE - 1);
    printf("Tiles around player\n");
    printf("    tile at (%d,%d): %d\n", (int)p.x, (int)p.y, tl.type);
    printf("    tile at (%d,%d): %d\n", (int)p.x + PLAYER_SIZE - 1, (int)p.y, tr.type);
    printf("    tile at (%d,%d): %d\n", (int)p.x, (int)p.y + PLAYER_SIZE - 1, bl.type);
    printf("    tile at (%d,%d): %d\n", (int)p.x + PLAYER_SIZE - 1, (int)p.y + PLAYER_SIZE - 1, br.type);
    printf("\n\n\n\n\nSPEED \t FRICTION \t JUMP \t GRAVITY\n");
    printf("\n%1.2f \t\t %1.2f \t\t %1.2f \t %+1.2f\n", p.speed, p.friction, p.jump, p.gravity);
}

int main() {
    time_t unixTime = time(NULL);
    srand((unsigned int) unixTime);
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
    resetPlayerDefaults(&player);

    MAP;
    generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        // circlePosition circlePad;
        // circleRead(&circlePad);

        if (kDown & KEY_START)
            break;
        if (player.canmove && kDown & KEY_L) {
            generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);
            resetPlayer(&player, spawnX, spawnY);
        }
        if (player.canmove && kDown & KEY_R || player.y > SCREEN_HEIGHT)
            resetPlayer(&player, spawnX, spawnY);
        if (kDown & KEY_X) {
            player.canmove = !player.canmove;
            printPauseScreen(player, map);
        }

        playerMovement(&player, map, kHeld & KEY_DLEFT, kHeld & KEY_DRIGHT, kHeld & KEY_DUP || kHeld & KEY_A);
        if (playerAttributeAdjustment(&player, kDown, kHeld, map)) printPauseScreen(player, map);
        
        if (player.x > TOP_WIDTH) {
            generateMap(map, spawnX, spawnY, caveHeight, wsChance, blockVariety, spawnBlock);
            resetPlayer(&player, spawnX, spawnY);
            allTimeCompleted++;
        }

        draw(top, map, player);
    }

    cfguExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
