#include "myLib.h"
#include "screens.h"

// Variables
SELECT wand;

// Sets up the wand selector for the splash state
void initSplash() {

    wand.choice = START;
}

// Handles every-frame changes of the wand in the splash state
void updateSplash() {

    // Change wand selection with arrow keys
    if (BUTTON_PRESSED(BUTTON_DOWN) && wand.choice == START) {
        wand.choice = INSTR;
    }
    if (BUTTON_PRESSED(BUTTON_UP) && wand.choice == INSTR) {
        wand.choice = START;
    }

    // Set wand screenRow and Col based on selection
    if (wand.choice == START) {
        wand.screenRow = 120;
        wand.screenCol = 2;
    } else if (wand.choice == INSTR) {
        wand.screenRow = 136;
        wand.screenCol = 2;
    }
}

// Draws the wand selector in the splash state
void drawSplash() {

    drawWand();
}

// Sets up the wand selector for the Instructions state
void initInstructions() {

    wand.choice = PLAY;
}

// Handles every-frame changes of the wand in the Instructions state
void updateInstructions() {

    // Change wand selection with arrow keys
    if (BUTTON_PRESSED(BUTTON_DOWN) && wand.choice == PLAY) {
        wand.choice = MENU;
    }
    if (BUTTON_PRESSED(BUTTON_UP) && wand.choice == MENU) {
        wand.choice = PLAY;
    }

    // Set wand screenRow and Col based on selection
    if (wand.choice == PLAY) {
        wand.screenRow = 115;
        wand.screenCol = 120;
    } else if (wand.choice == MENU) {
        wand.screenRow = 127;
        wand.screenCol = 120;
    }
}

// Draws the wand selector in the Instructions state
void drawInstructions() {

    // Draw wand in black instead of grey
    shadowOAM[29].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(8, 26);
    shadowOAM[29].attr0 = (ROWMASK & wand.screenRow) | ATTR0_SQUARE;
    shadowOAM[29].attr1 = (COLMASK & wand.screenCol) | ATTR1_SMALL;
}

// Sets up the wand selector for the Pause state
void initPause() {

    wand.choice = RESUME;
}

// Handles every-frame changes of the wand in the Pause state
void updatePause() {

    // Change wand selection with arrow keys
    if (BUTTON_PRESSED(BUTTON_DOWN) && wand.choice == RESUME) {
        wand.choice = QUIT;
    }
    if (BUTTON_PRESSED(BUTTON_UP) && wand.choice == QUIT) {
        wand.choice = RESUME;
    }

    // Set wand screenRow and Col based on selection
    if (wand.choice == RESUME) {
        wand.screenRow = 103;
        wand.screenCol = 81;
    } else if (wand.choice == QUIT) {
        wand.screenRow = 119;
        wand.screenCol = 81;
    }
}

// Draws the wand selector in the Pause state
void drawPause() {

    drawWand();
}

// Draws the wand
void drawWand() {

    shadowOAM[29].attr2 = ATTR2_PALROW(2) | ATTR2_TILEID(8, 26);
    shadowOAM[29].attr0 = (ROWMASK & wand.screenRow) | ATTR0_SQUARE;
    shadowOAM[29].attr1 = (COLMASK & wand.screenCol) | ATTR1_SMALL;
}
