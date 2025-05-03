#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define TILE_SIZE  16
#define BG_COLOR    1
#define PLAYER_SIZE 5
#define GRAVITY    -0.64
#define FRICTION    0.64
#define JUMP       -6.16
#define SPEED       0.68

#define PLAYER_Z     1.0f
#define TILE_Z       0.5f
#define BACKGROUND_Z 0.0f

#define DEFAULT_SPAWNX TILE_SIZE/2 + PLAYER_SIZE/2
#define DEFAULT_SPAWNY GSP_SCREEN_HEIGHT_TOP/2 - PLAYER_SIZE/2
#define DEFAULT_SPAWN_BLOCK     1
#define DEFAULT_CAVE_HEIGHT     0
#define DEFAULT_WS_CHANCE      65
#define DEFAULT_BLOCK_VARIETY  75
#define DEFAULT_SHOW_TEST_TILES 0

#define PLAYER_COLOR     C2D_Color32(255,   0,   0, 255)
#define BACKGROUND_COLOR C2D_Color32(222, 222, 222, 255)
#define TILE_COLOR       C2D_Color32(  0,   0,   0, 255)


struct Player {
	float x, y, dx, dy;
};


// initialize map info
int spawnX       = DEFAULT_SPAWNX;
int spawnY       = DEFAULT_SPAWNY;
int spawnBlock   = DEFAULT_SPAWN_BLOCK;
int caveHeight   = DEFAULT_CAVE_HEIGHT;
int wsChance     = DEFAULT_WS_CHANCE;
int blockVariety = DEFAULT_BLOCK_VARIETY;

int showTestTiles = DEFAULT_SHOW_TEST_TILES;


void drawPlayer(int x, int y) {
	C2D_DrawRectSolid(x, y, 0.5f, PLAYER_SIZE, PLAYER_SIZE, PLAYER_COLOR);
}

void resetPlayer(struct Player *player) {
	player->x = spawnX;
	player->y = spawnY;
	player->dx = 0;
	player->dy = 0;
}

void drawTile(int x, int y, int type) {
	switch (type) {
		case 0:
		default:
			C2D_DrawRectSolid(x, y, 0.5f, TILE_SIZE, TILE_SIZE, TILE_COLOR);
			break;
	}
}


int main(int argc, char **argv)
{
	// Initialize services
	gfxInitDefault();
	if (!C2D_Init(C2D_DEFAULT_MAX_OBJECTS)) {
		gfxExit();
		return -1;
	}

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_BOTTOM, NULL);

	printf("\x1b[30;16HPress Start to exit.");


	// initialize player
	struct Player player;
	resetPlayer(&player);

	// Main loop
	while (aptMainLoop()) {
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		// fill background
		C2D_DrawRectSolid(0, 0, BACKGROUND_Z, GSP_SCREEN_WIDTH, GSP_SCREEN_HEIGHT_TOP, BACKGROUND_COLOR);

		// draw base line of tiles
		for (int x = 0; x < GSP_SCREEN_WIDTH; x += TILE_SIZE) {
			drawTile(x, (int)(GSP_SCREEN_HEIGHT_TOP/2) - TILE_SIZE, 0);
		}

		// player controls
		if (kHeld & KEY_DRIGHT) {
			player.x += 2;
		} else if (kHeld & KEY_DLEFT) {
			player.x -= 2;
		}
		drawPlayer(player.x, player.y);


		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Exit services
	gfxExit();
	C2D_Fini();

	return 0;
}
