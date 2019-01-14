#include "myLib.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "Item.h"

// Variables
int hOff;
int vOff;
int hOffRiver;
OBJ_ATTR shadowOAM[128];  // 0-2: collection, 3-5: cloak status, 6-8: lives, 9-14: spellStatus, 15: player, 16-21: enemies, 22-24: items, 25-27: spells, 28: exit, 29: wand selecter
int itemsCollected;
ITEM items[ITEMCOUNT];
ITEM collection[ITEMCOUNT];
ITEM exitLevel;
int switchLevels;
int resetLevel;
int gameOver;
int cheatOn;
int riverCounter;

// Initialize the game
void initGame() {

    // Put general game initialization here
    switchLevels = 0;
    resetLevel = 0;
    gameOver = 0;
    cheatOn = 0;
    livesRemaining = LIVES;

    initItems();
    initCollection();
    initLives();
}

// Initialize Level 1
void initLevel1() {

    // Put initialization specific to Level 1 here
    switchLevels = 0;
    resetLevel = 0;
    items[LEVEL1].collected = 0;

    // Place screen on map
    vOff = 12;
    hOff = 9;

    initPlayer();
    initEnemies();
    initExit();
}

// Initialize Level 2
void initLevel2() {

    // Put initialization specific to Level 2 here
    switchLevels = 0;
    resetLevel = 0;
    items[LEVEL2].collected = 0;

    // Place screen on map
    vOff = 250;
    hOff = 225;

    initPlayer();
    initEnemies();
    initExit();
}

// Initialize Level 1
void initLevel3() {

    // Put initialization specific to Level 3 here
    switchLevels = 0;
    resetLevel = 0;
    items[LEVEL3].collected = 0;

    // Place screen on map
    vOff = 352;
    hOff = 111;
    hOffRiver = 0;

    riverCounter = 0;

    initPlayer();
    initEnemies();
    initExit();
}

// Updates the game each frame
void updateGame() {

	updatePlayer();
    updateEnemies();
    updateItems();
    updateCollection();
    updateExit();
    updateLives();

    // Check for cheat
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        cheatOn = 1;
    }

    // Move the river on level 3 at half the speed of the player
    if (level == LEVEL3) {
        riverCounter++;
        if (riverCounter % 2 == 0) {
            hOffRiver -= 1;
        }
    }
}

// Draws the game each frame
void drawGame() {

    drawPlayer();
    drawEnemies();
    drawItems();
    drawCollection();
    drawExit();
    drawLives();
}

// Initializes the item parameters
void initItems() {

    // Level 1 Item
    items[LEVEL1].worldRow = 99;
    items[LEVEL1].worldCol = 43;
    items[LEVEL1].width = 16;
    items[LEVEL1].height = 18;
    items[LEVEL1].hide = 1;
    items[LEVEL1].collected = 0;

    // Level 2 Item
    items[LEVEL2].worldRow = 406;
    items[LEVEL2].worldCol = 439;
    items[LEVEL2].width = 24;
    items[LEVEL2].height = 18;
    items[LEVEL2].hide = 1;
    items[LEVEL2].collected = 0;

    // Level 3 Item
    items[LEVEL3].worldRow = 48;
    items[LEVEL3].worldCol = 398;
    items[LEVEL3].width = 18;
    items[LEVEL3].height = 24;
    items[LEVEL3].hide = 1;
    items[LEVEL3].collected = 0;
}

// Updates the item for the current level
void updateItems() {

    // Check for collection by player
    if (!items[level].collected && collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                items[level].worldRow, items[level].worldCol, items[level].height, items[level].width)) {
        items[level].collected = 1;
        playSoundB(Item,ITEMLEN,ITEMFREQ, 0);
    }

    // Show the item for the current level if it hasn't been collected yet
    for (int i = 0; i < ITEMCOUNT; i++) {
        if (i == level && !items[i].collected) {
            items[i].hide = 0;
        } else {
            items[i].hide = 1;
        }
    }

    // Update screen row and screen col
    items[level].screenRow = items[level].worldRow - vOff;
    items[level].screenCol = items[level].worldCol - hOff;

    // Hide if off screen to avoid display problems
    if (items[level].screenRow + items[level].height < 0 || items[level].screenRow > 160
            || items[level].screenCol + items[level].width < 0 || items[level].screenCol > 240) {
        items[level].hide = 1;
    }
}

// Draws or hides the items as necessary
void drawItems() {

    for (int i = 0; i < ITEMCOUNT; i++) {
        if (items[i].hide) {
            shadowOAM[i + 22].attr0 |= ATTR0_HIDE;
        } else {

            shadowOAM[i + 22].attr0 = (ROWMASK & items[i].screenRow) | ATTR0_SQUARE;
            shadowOAM[i + 22].attr1 = (COLMASK & items[i].screenCol) | ATTR1_MEDIUM;

            // Level 1
            if (level == LEVEL1) {
                shadowOAM[i + 22].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(0, 28);
            }
            // Level 2
            if (level == LEVEL2) {
                shadowOAM[i + 22].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(0, 24);
            }
            // Level 3
            if (level == LEVEL3) {
                shadowOAM[i + 22].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(4, 24);
            }
        }
    }
}

// Sets up the collection display in the bottom right corner
void initCollection() {

    // Level 1 Item
    collection[LEVEL1].screenRow = 141;
    collection[LEVEL1].screenCol = 189;
    collection[LEVEL1].width = 16;
    collection[LEVEL1].height = 16;
    collection[LEVEL1].collected = 0;

    // Level 2 Item
    collection[LEVEL2].screenRow = 141;
    collection[LEVEL2].screenCol = 205;
    collection[LEVEL2].width = 16;
    collection[LEVEL2].height = 16;
    collection[LEVEL2].collected = 0;

    // Level 3 Item
    collection[LEVEL3].screenRow = 141;
    collection[LEVEL3].screenCol = 222;
    collection[LEVEL3].width = 16;
    collection[LEVEL3].height = 16;
    collection[LEVEL3].collected = 0;
}

// Updates the collection display every frame
void updateCollection() {

    // Update currently collected items
    for (int i = 0; i < ITEMCOUNT; i++) {

        if(items[i].collected) {
            collection[i].collected = 1;
        } else {
            collection[i].collected = 0;
        }
    }
}

// Draws the collection display
void drawCollection() {

    // Level 1 Item
    shadowOAM[0].attr0 = (ROWMASK & collection[0].screenRow) | ATTR0_SQUARE;
    shadowOAM[0].attr1 = (COLMASK & collection[0].screenCol) | ATTR1_SMALL;

    // Draw grey shape if not yet collected
    if (!collection[0].collected) {
        shadowOAM[0].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(4, 30);
    } else {  // Draw colorful shape
        shadowOAM[0].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(4, 28);
    }

    // Level 2 Item
    shadowOAM[1].attr0 = (ROWMASK & collection[1].screenRow) | ATTR0_SQUARE;
    shadowOAM[1].attr1 = (COLMASK & collection[1].screenCol) | ATTR1_SMALL;

    // Draw grey shape if not yet collected
    if (!collection[1].collected) {
        shadowOAM[1].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(6, 30);
    } else {  // Draw colorful shape
        shadowOAM[1].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(6, 28);
    }

    // Level 3 Item
    shadowOAM[2].attr0 = (ROWMASK & collection[2].screenRow) | ATTR0_SQUARE;
    shadowOAM[2].attr1 = (COLMASK & collection[2].screenCol) | ATTR1_SMALL;

    // Draw grey shape if not yet collected
    if (!collection[2].collected) {
        shadowOAM[2].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(8, 30);
    } else {  // Draw colorful shape
        shadowOAM[2].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(8, 28);
    }
}

// Sets up the exit from the level
void initExit() {

    // Level 1 location
    if (level == LEVEL1) {
        exitLevel.worldRow = 216;
        exitLevel.worldCol = 103;
        exitLevel.height = 40;
        exitLevel.width = 48;
    }

    // Level 2 location
    if (level == LEVEL2) {
        exitLevel.worldRow = 19;
        exitLevel.worldCol = 327;
        exitLevel.height = 48;
        exitLevel.width = 32;
    }

    // Level 3 location
    if (level == LEVEL3) {
        exitLevel.worldRow = 10;
        exitLevel.worldCol = 42;
        exitLevel.height = 32;
        exitLevel.width = 32;
    }

    exitLevel.hide = 1;
}

// Handles every-frame updates of the exit
void updateExit() {

    // Show exit if item has been collected
    if (items[level].collected) {
        exitLevel.hide = 0;
    } else {
        exitLevel.hide = 1;
    }

    // Check for player leaving, if exit not hidden
    if (exitLevel.hide == 0 && collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                                exitLevel.worldRow, exitLevel.worldCol, exitLevel.height, exitLevel.width)) {
        switchLevels = 1;
    }

    // Update screen row and screen col
    exitLevel.screenRow = exitLevel.worldRow - vOff;
    exitLevel.screenCol = exitLevel.worldCol - hOff;

    // Hide if off screen to avoid display problems
    if (exitLevel.screenRow + exitLevel.height < 0 || exitLevel.screenRow > 160
            || exitLevel.screenCol + exitLevel.width < 0 || exitLevel.screenCol > 240) {
        exitLevel.hide = 1;
    }
}

// Draw the exit
void drawExit() {

    if (exitLevel.hide) {
        shadowOAM[28].attr0 |= ATTR0_HIDE;
    } else {

        // Level 1
        if (level == LEVEL1) {
            shadowOAM[28].attr2 = ATTR2_PALROW(2) | ATTR2_TILEID(10, 24);
            shadowOAM[28].attr0 = (ROWMASK & exitLevel.screenRow) | ATTR0_SQUARE;
            shadowOAM[28].attr1 = (COLMASK & exitLevel.screenCol) | ATTR1_LARGE;
        }
        // Level 2
        if (level == LEVEL2) {
            shadowOAM[28].attr2 = ATTR2_PALROW(2) | ATTR2_TILEID(2, 16);
            shadowOAM[28].attr0 = (ROWMASK & exitLevel.screenRow) | ATTR0_TALL;
            shadowOAM[28].attr1 = (COLMASK & exitLevel.screenCol) | ATTR1_LARGE;
        }
        // Level 3
        if (level == LEVEL3) {
            shadowOAM[28].attr2 = ATTR2_PALROW(4) | ATTR2_TILEID(0, 12);
            shadowOAM[28].attr0 = (ROWMASK & exitLevel.screenRow) | ATTR0_SQUARE;
            shadowOAM[28].attr1 = (COLMASK & exitLevel.screenCol) | ATTR1_MEDIUM;
        }
    }
}
