/**

Cheat: press select to get extended invisibility

See instructions screen for how to play.

**/

#include <stdlib.h>
#include "myLib.h"
#include "screens.h"
#include "game.h"
#include "spritesheet.h"
#include "spritesheet2.h"
#include "spritesheet3.h"
#include "splash.h"
#include "instructions.h"
#include "potions.h"
#include "corridors.h"
#include "forest.h"
#include "river.h"
#include "pause.h"
#include "win.h"
#include "lose.h"
#include "ThemeSong.h"

// Prototypes
void initialize();
void goToSplash();
void splash();
void goToInstructions();
void instructions();
void goToGame1();
void game1();
void goToGame2();
void game2();
void goToGame3();
void game3();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();

// TODO: alpha blending for transitions?
// TODO: breaking heart puopup when life lost
// TODO: heart design

/*
Play Testing

Mirela
- Used invisibility on ghosts even though it didn't work
- Wanted to stun ghosts then got stuck (fixed, stunning ghosts causes them to disappear)
- First ghost hard to get past (increased pause time, test with others)
- Confused about exit on 2nd level (fixed)
- Lives would be good to implement
- Don't know about 3rd level exit, didn't get there
- Spiders effectively scary
- Returning from lose screen seemed to be intuitive

Helen
- Adding lives or checkpoints would be good
- Doors still confusing (level 2)
- Status bars unclear
- Still thinks collisions too far
- Returning to menu intuitive

Crystal
- Didn't read instructions or use abilities at first
- Leaving lose screen confusing
- Gave up on 2nd level
- Really wants checkpoints/lives
*/


// Art credits: castle corridors (Kefin Castle) by Davias, characters by Sindorman,
// spider by Barubary, forest by Davias, cloak by Mr. J

// States
enum {SPLASH, INSTRUCTIONS, GAME1, GAME2, GAME3, PAUSE, WIN, LOSE};
int state;
int prevState;

// Level Specific Constants and Variables
int level;
int mapHeight;
int mapWidth;

// Button Variables
unsigned short buttons;
unsigned short oldButtons;

// Random Seed
int seed;

int main() {

    initialize();

    while(1) {

        // Update button variables
        oldButtons = buttons;
        buttons = BUTTONS;

        // State Machine
        switch(state) {

            case SPLASH:
                splash();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case GAME1:
                game1();
                break;
            case GAME2:
                game2();
                break;
            case GAME3:
                game3();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
        }
    }
}

// Sets up GBA
void initialize() {

    REG_DISPCTL = MODE0;

    buttons = BUTTONS;

    // Setup sounds
    setupSounds();
    setupInterrupts();
    playSoundA(ThemeSong,THEMESONGLEN - 3000,THEMESONGFREQ, 1);

    // Set up the first state
    prevState = SPLASH;
    goToSplash();
}

// Sets up the splash state
void goToSplash() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // Load the Background's Palette and the Tiles/Map
    DMANow(3, splashPal, PALETTE, 256);
    DMANow(3, splashTiles, &CHARBLOCK[0], splashTilesLen/2);
    DMANow(3, splashMap, &SCREENBLOCK[31], splashMapLen/2);
    REG_BG0VOFF = 0;
    REG_BG0HOFF = 0;

    // Set up the first sprite sheet
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    initSplash();

    prevState = state;
    state = SPLASH;
    seed = 0;
}

// Runs every frame of the splash state
void splash() {

    seed++;

    updateSplash();
    drawSplash();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        if (wand.choice == INSTR) {
            goToInstructions();
        } else if (wand.choice == START) {

            // Seed the random generator
            srand(seed);

            initGame();
            goToGame1();
        }
    }
}

// Sets up the instructions state
void goToInstructions() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // Load the Background's Palette and the Tiles/Map
    DMANow(3, instructionsPal, PALETTE, 256);
    DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen/2);
    DMANow(3, instructionsMap, &SCREENBLOCK[31], instructionsMapLen/2);
    REG_BG0VOFF = 0;
    REG_BG0HOFF = 0;

    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    initInstructions();

    prevState = state;
    state = INSTRUCTIONS;
}

// Runs every frame of the instructions state
void instructions() {

    updateInstructions();
    drawInstructions();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        if (wand.choice == MENU) {
            goToSplash();
        } else if (wand.choice == PLAY) {

            // Seed the random generator
            srand(seed);

            initGame();
            goToGame1();
        }
    }
}


// Sets up the game1 state (first level)
void goToGame1() {

    // Make sure changes do not appear onscreen
    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);
    REG_BLDCNT = BLD_STD | BLD_BOT0;
    REG_BLDALPHA = BLD_EVA(10) | BLD_EVB(10);

    // Set up the potions background
    DMANow(3, potionsPal, PALETTE, 256);
    DMANow(3, potionsTiles, &CHARBLOCK[0], potionsTilesLen/2);
    DMANow(3, potionsMap, &SCREENBLOCK[31], potionsMapLen/2);
    REG_BG0VOFF = vOff;
    REG_BG0HOFF = hOff;
    mapHeight = 256;
    mapWidth = 256;

    // Set up the sprites
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    // Initalize Level 1
    prevState = state;
    state = GAME1;
    level = LEVEL1;
    // Only reset screen and player to level starting locations if coming from
    // previous level, not pause
    if (prevState != PAUSE) {
        initLevel1();
    }
}

// Runs every frame of the game1 state
void game1() {

    updateGame();
    drawGame();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    } else if (gameOver) {
        goToLose();
    } else if (switchLevels) {
        goToGame2();
    } else if (resetLevel) {
        goToGame1();
    }
}

// Sets up the game2 state (second level)
void goToGame2() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_LARGE | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28);
    REG_BLDCNT = BLD_STD | BLD_BOT0;
    REG_BLDALPHA = BLD_EVA(10) | BLD_EVB(10);

    // Set up the corridors background
    DMANow(3, corridorsPal, PALETTE, 256);
    DMANow(3, corridorsTiles, &CHARBLOCK[0], corridorsTilesLen/2);
    DMANow(3, corridorsMap, &SCREENBLOCK[28], corridorsMapLen/2);
    REG_BG0VOFF = vOff;
    REG_BG0HOFF = hOff;
    mapHeight = 512;
    mapWidth = 512;

    // Set up the sprites
    DMANow(3, spritesheet2Tiles, &CHARBLOCK[4], spritesheet2TilesLen/2);
    DMANow(3, spritesheet2Pal, SPRITEPALETTE, spritesheet2PalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    // Initialize Level 2
    prevState = state;
    state = GAME2;
    level = LEVEL2;
    // Only reset screen and player to level starting locations if coming from
    // previous level, not pause
    if (prevState != PAUSE) {
        initLevel2();
    }
}

// Runs every frame of the game2 state
void game2() {

    updateGame();
    drawGame();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    } else if (gameOver) {
        goToLose();
    } else if (switchLevels) {
        goToGame3();
    } else if (resetLevel) {
        goToGame2();
    }
}

// Sets up the game3 state (third level)
void goToGame3() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_LARGE | BG_4BPP | BG_CHARBLOCK(1) | BG_SCREENBLOCK(24);
    REG_BG1CNT = BG_SIZE_LARGE | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28);
    REG_BLDCNT = BLD_STD | BLD_BOT0 | BLD_BOT1;
    REG_BLDALPHA = BLD_EVA(10) | BLD_EVB(10);

    // Set up the corridors background
    DMANow(3, forestPal, PALETTE, 256);
    DMANow(3, forestTiles, &CHARBLOCK[1], forestTilesLen/2);
    DMANow(3, forestMap, &SCREENBLOCK[24], forestMapLen/2);
    DMANow(3, riverTiles, &CHARBLOCK[0], riverTilesLen/2);
    DMANow(3, riverMap, &SCREENBLOCK[28], riverMapLen/2);
    REG_BG0VOFF = vOff;
    REG_BG0HOFF = hOff;
    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOffRiver;
    mapHeight = 512;
    mapWidth = 512;

    // Set up the sprites
    DMANow(3, spritesheet3Tiles, &CHARBLOCK[4], spritesheet3TilesLen/2);
    DMANow(3, spritesheet3Pal, SPRITEPALETTE, spritesheet3PalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    // Initialize Level 3
    prevState = state;
    state = GAME3;
    level = LEVEL3;
    // Don't reset screen and player to level starting locations if coming from
    // pause
    if (prevState != PAUSE) {
        initLevel3();
    }
}

// Runs every frame of the game3 state
void game3() {

    updateGame();
    drawGame();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;
    REG_BG1HOFF = hOffRiver;
    REG_BG1VOFF = vOff;

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    } else if (gameOver) {
        goToLose();
    } else if (switchLevels) {
        goToWin();
    } else if (resetLevel) {
        goToGame3();
    }
}

// Sets up the pause state
void goToPause() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // Load the Background's Palette and the Tiles/Map
    DMANow(3, pausePal, PALETTE, 256);
    DMANow(3, pauseTiles, &CHARBLOCK[0], pauseTilesLen/2);
    DMANow(3, pauseMap, &SCREENBLOCK[31], pauseMapLen/2);
    REG_BG0VOFF = 0;
    REG_BG0HOFF = 0;

    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    initPause();

    prevState = state;
    state = PAUSE;
}

// Runs every frame of the pause state
void pause() {

    updatePause();
    drawPause();

    // Lock the framerate to 60 fps
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        if (wand.choice == RESUME) {
            if (prevState == GAME1) {
                goToGame1();
            } else if (prevState == GAME2) {
                goToGame2();
            } else {
                goToGame3();
            }
        }
        if (wand.choice == QUIT) {
            goToSplash();
        }
    }
}

// Sets up the win state
void goToWin() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // Load the Background's Palette and the Tiles/Map
    DMANow(3, winPal, PALETTE, 256);
    DMANow(3, winTiles, &CHARBLOCK[0], winTilesLen/2);
    DMANow(3, winMap, &SCREENBLOCK[31], winMapLen/2);
    REG_BG0VOFF = 0;
    REG_BG0HOFF = 0;

    prevState = state;
    state = WIN;
}

// Runs every frame of the win state
void win() {

    // Lock the framerate to 60 fps
    waitForVBlank();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToSplash();
    }
}

// Sets up the lose state
void goToLose() {

    waitForVBlank();

    // Set up control registers
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_4BPP | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // Load the Background's Palette and the Tiles/Map
    DMANow(3, losePal, PALETTE, 256);
    DMANow(3, loseTiles, &CHARBLOCK[0], loseTilesLen/2);
    DMANow(3, loseMap, &SCREENBLOCK[31], loseMapLen/2);
    REG_BG0VOFF = 0;
    REG_BG0HOFF = 0;

    prevState = state;
    state = LOSE;
}

// Runs every frame of the lose state
void lose() {

    // Lock the framerate to 60 fps
    waitForVBlank();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToSplash();
    }
}
