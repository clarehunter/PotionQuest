#include "myLib.h"
#include "player.h"
#include "movement.h"
#include "Spell.h"

// Variables
ANISPRITE harry;
SPELL stupefy[SPELLARSENAL];
int spellsRemaining;
int spellTimer;
int cloakOn;
int cloakTimer;
int recharging;
STATUS lives[LIVES];
int livesRemaining;
STATUS bar;
STATUS cheat;
STATUS spellStatus[SPELLCOUNT];

// Initialize the player
void initPlayer() {

    harry.width = 28;
    harry.height = 32;
    harry.rdel = 1;
    harry.cdel = 1;

    // Place in the middle of the screen in the world location chosen earlier
    harry.worldRow = SCREENHEIGHT/2-harry.width/2 + vOff;
    harry.worldCol = SCREENWIDTH/2-harry.height/2 + hOff;
    if (level == LEVEL3) {
        harry.worldRow += 50;
    }
    harry.aniCounter = 0;
    harry.curFrame = 1;
    harry.numFrames = 3;
    harry.aniState = FRONT;

    // Initialize spells and cloak
    initSpells();
    initCloak();
    initStatus();
}

// Handle every-frame actions of the player
void updatePlayer() {

    if(BUTTON_HELD(BUTTON_UP)) {
        if (canMoveUp(&harry)) {

            // Update harry's world position
            harry.worldRow -= harry.rdel;

            /*make sure the background offset doesn't show past the edge,
                and only update the offset variables if harry is in the right spot*/
            if (vOff > 0 && harry.screenRow + harry.height/2 <= SCREENHEIGHT / 2) {

                // Update background offset variable if the above is true
                vOff -= harry.rdel;
            }
        }
    }
    if(BUTTON_HELD(BUTTON_DOWN)) {
        if (canMoveDown(&harry)) {

            // Update harry's world position
            harry.worldRow += harry.rdel;

            /*Make sure the background offset doesn't show past the edge,
                and only update the offset variables if harry is in the right spot*/
            if (vOff + SCREENHEIGHT + 1 < mapHeight && harry.screenRow + harry.height/2 >= SCREENHEIGHT / 2) {

                // Update background offset variable if the above is true
                vOff += harry.rdel;
            }
        }
    }
    if(BUTTON_HELD(BUTTON_LEFT)) {
        if (canMoveLeft(&harry)) {

            // Update harry's world position
            harry.worldCol -= harry.cdel;

            /*Make sure the background offset doesn't show past the edge,
                and only update the offset variables if harry is in the right spot*/
            if (hOff > 0 && harry.screenCol + harry.width/2 <= SCREENWIDTH / 2) {

                // Update background offset variable if the above is true
                hOff -= harry.cdel;
                if (level == LEVEL3) {
                    hOffRiver -= harry.cdel;
                }
            }
        }
    }
    if(BUTTON_HELD(BUTTON_RIGHT)) {
        if (canMoveRight(&harry)) {

            // Update harry's world position
            harry.worldCol += harry.cdel;

            /*Make sure the background offset doesn't show past the edge,
                and only update the offset variables if harry is in the right spot*/
            if (hOff + SCREENWIDTH + 1 < mapWidth && harry.screenCol + harry.width/2 >= SCREENWIDTH / 2) {

                // Update background offset variable if the above is true
                hOff += harry.cdel;
                if (level == LEVEL3) {
                    hOffRiver += harry.cdel;
                }
            }
        }
    }

    // Check for casting a spell
    if (BUTTON_PRESSED(BUTTON_A)) {
        castSpell();
    }

    // Put on invisibility cloak
    if (BUTTON_PRESSED(BUTTON_B)) {
        cloak();
    }

    // Update screen row and screen col
    harry.screenRow = harry.worldRow - vOff;
    harry.screenCol = harry.worldCol - hOff;

    updateSpells();
    updateCloak();
    animatePlayer();
    updateStatus();
}

// Handle player animation states
void animatePlayer() {

    // Set previous state to current state
    harry.prevAniState = harry.aniState;
    harry.aniState = IDLE;

    // Change the animation frame every 20 frames of gameplay
    if(harry.aniCounter % 20 == 0) {
        harry.curFrame = (harry.curFrame + 1) % harry.numFrames;
    }

    // Control movement and change animation state
    if(BUTTON_HELD(BUTTON_UP))
        harry.aniState = BACK;
    if(BUTTON_HELD(BUTTON_DOWN))
        harry.aniState = FRONT;
    if(BUTTON_HELD(BUTTON_LEFT))
        harry.aniState = LEFT;
    if(BUTTON_HELD(BUTTON_RIGHT))
        harry.aniState = RIGHT;

    // If the harry aniState is idle, frame is harry standing
    if (harry.aniState == IDLE) {
        harry.curFrame = 1;
        harry.aniCounter = 0;
        harry.aniState = harry.prevAniState;
    } else {
        harry.aniCounter++;
    }
}

// Draw the player
void drawPlayer() {

    if (harry.hide) {
        shadowOAM[15].attr0 |= ATTR0_HIDE;
    } else {

        // Alpha blending to indicate invisibility cloak
        if (cloakOn) {
            shadowOAM[15].attr0 = (ROWMASK & harry.screenRow) | ATTR0_SQUARE | ATTR0_BLEND;
        } else {
            shadowOAM[15].attr0 = (ROWMASK & harry.screenRow) | ATTR0_SQUARE | ATTR0_NOBLEND;
        }
        shadowOAM[15].attr1 = (COLMASK & harry.screenCol) | ATTR1_MEDIUM;
        shadowOAM[15].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(harry.aniState * 4, harry.curFrame * 4);
    }

    drawSpells();
    drawStatus();
}

// Sets up the spells
void initSpells() {

    spellsRemaining = SPELLCOUNT;
    spellTimer = 40;

    for (int i = 0; i < SPELLARSENAL; i++) {

        stupefy[i].worldRow = harry.worldRow;
        stupefy[i].worldCol = harry.worldCol;
        stupefy[i].rdel = 2;
        stupefy[i].cdel = 2;
        stupefy[i].height = 16;
        stupefy[i].width = 16;
        stupefy[i].curFrame = 0;
        stupefy[i].numFrames = 2;
        stupefy[i].aniCounter = 0;
        stupefy[i].active = 0;
    }
}

// Casts stupefy in the direction player is facing
void castSpell() {

    // Only cast a spell if not currently wearing the cloak,
    // and there is at least one spell left
    if (spellsRemaining && !cloakOn && spellTimer >= 40) {

        playSoundB(Spell,SPELLLEN,SPELLFREQ, 0);

        // Find an inactive spell
        int i = 0;
        while (stupefy[i].active) {
            i++;
        }

        // Point spell in direction player is currently facing and set correct starting location
        if (harry.aniState == BACK) {
            stupefy[i].rdel = -2;
            stupefy[i].cdel = 0;
            stupefy[i].aniState = BACK;
            stupefy[i].worldRow = harry.worldRow;
            stupefy[i].worldCol = harry.worldCol + (harry.width / 2) - 3;
        } else if (harry.aniState == FRONT) {
            stupefy[i].rdel = 2;
            stupefy[i].cdel = 0;
            stupefy[i].aniState = FRONT;
            stupefy[i].worldRow = harry.worldRow + harry.height - (stupefy[i].height / 2);
            stupefy[i].worldCol = harry.worldCol + (harry.width / 2) - 2;
        } else if (harry.aniState == LEFT) {
            stupefy[i].rdel = 0;
            stupefy[i].cdel = -2;
            stupefy[i].aniState = LEFT;
            stupefy[i].worldRow = harry.worldRow + (harry.height / 2);
            stupefy[i].worldCol = harry.worldCol;
        } else if (harry.aniState == RIGHT) {
            stupefy[i].rdel = 0;
            stupefy[i].cdel = 2;
            stupefy[i].aniState = RIGHT;
            stupefy[i].worldRow = harry.worldRow + (harry.height / 2);
            stupefy[i].worldCol = harry.worldCol + (harry.width / 2) - (stupefy[i].width / 2);
        }

        // Update screen row and screen col
        stupefy[i].screenRow = stupefy[i].worldRow - vOff;
        stupefy[i].screenCol = stupefy[i].worldCol - hOff;

        stupefy[i].active = 1;
        spellsRemaining--;
        spellTimer = 0;
    }
}

// Handle every-frame actions of the spells
void updateSpells() {

    for (int i = 0; i < SPELLARSENAL; i++) {

        if (stupefy[i].active) {

            // Make width and height more accurate
            if (stupefy[i].aniState == FRONT) {
                stupefy[i].width = 5;
                stupefy[i].height = 16;
            } else if (stupefy[i].aniState == LEFT) {
                stupefy[i].width = 16;
                stupefy[i].height = 5;
            } else if (stupefy[i].aniState == RIGHT) {
                stupefy[i].width = 16;
                stupefy[i].height = 5;
            } else if (stupefy[i].aniState == BACK) {
                stupefy[i].width = 5;
                stupefy[i].height = 16;
            }

            // Update world position
            stupefy[i].worldRow += stupefy[i].rdel;
            stupefy[i].worldCol += stupefy[i].cdel;

            // Update screen row and screen col
            stupefy[i].screenRow = stupefy[i].worldRow - vOff;
            stupefy[i].screenCol = stupefy[i].worldCol - hOff;

            // If spell goes off screen, hide
            if (stupefy[i].screenRow + stupefy[i].height < 0 || stupefy[i].screenRow > 160
                    || stupefy[i].screenCol + stupefy[i].width < 0 || stupefy[i].screenCol > 240) {
                stupefy[i].active = 0;
            }
        }
    }

    animateSpells();
    spellTimer++;
}

// Animates the spelles
void animateSpells() {

    for (int i = 0; i < SPELLARSENAL; i++) {

        if (stupefy[i].active) {

            // Change the animation frame every 20 frames of gameplay
            if(stupefy[i].aniCounter % 20 == 0) {
                stupefy[i].curFrame = (stupefy[i].curFrame + 1) % stupefy[i].numFrames;
            }

            stupefy[i].aniCounter++;
        }
    }
}

// Draws the spells on the screen
void drawSpells() {

    for (int i = 0; i < SPELLARSENAL; i++) {

        if (!stupefy[i].active) {
            shadowOAM[25 + i].attr0 |= ATTR0_HIDE;
        } else {

            if (stupefy[i].aniState == FRONT) {
                shadowOAM[25 + i].attr0 = (ROWMASK & stupefy[i].screenRow) | ATTR0_TALL;
                shadowOAM[25 + i].attr1 = (COLMASK & stupefy[i].screenCol) | ATTR1_TINY;
                shadowOAM[25 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(7, stupefy[i].curFrame + 20);
            } else if (stupefy[i].aniState == BACK) {
                shadowOAM[25 + i].attr0 = (ROWMASK & stupefy[i].screenRow) | ATTR0_TALL;
                shadowOAM[25 + i].attr1 = (COLMASK & stupefy[i].screenCol) | ATTR1_TINY;
                shadowOAM[25 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(7, stupefy[i].curFrame + 22);
            } else if (stupefy[i].aniState == LEFT) {
                shadowOAM[25 + i].attr0 = (ROWMASK & stupefy[i].screenRow) | ATTR0_WIDE;
                shadowOAM[25 + i].attr1 = (COLMASK & stupefy[i].screenCol) | ATTR1_TINY;
                shadowOAM[25 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(9, (stupefy[i].curFrame * 2) + 20);
            } else {  // RIGHT
                shadowOAM[25 + i].attr0 = (ROWMASK & stupefy[i].screenRow) | ATTR0_WIDE;
                shadowOAM[25 + i].attr1 = (COLMASK & stupefy[i].screenCol) | ATTR1_TINY;
                shadowOAM[25 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(10, (stupefy[i].curFrame * 2) + 20);
            }
        }
    }
}

// Sets up the invisibility cloak
void initCloak() {

    cloakOn = 0;
    cloakTimer = 0;
    recharging = 0;
}

// Turns on invisibility cloak
void cloak() {

    // Only turn on cloak if not currently on or recharging from last time on
    if (!cloakOn && !recharging) {
        cloakOn = 1;
    }
}

// Handles every-frame updates for the cloak
void updateCloak() {

    // If the cloak is currently on, count frames until its time to turn it off,
    // then start recharging
    if (cloakOn) {

        cloakTimer += 1;

        int time;
        if (cheatOn) {
            time = SUPERCLOAKFRAMES;
        } else {
            time = CLOAKFRAMES;
        }
        if (cloakTimer > time) {
            cloakOn = 0;
            recharging = 1;
            cloakTimer = 0;
            bar.curFrame = 4;
        }
    }

    // Count frames until the cloak is allowed to be used again
    if (recharging) {

        cloakTimer += 1;

        int time;
        if (cheatOn) {
            time = SUPERCLOAKRECHARGE;
        } else {
            time = CLOAKRECHARGE;
        }
        if (cloakTimer > time) {
            recharging = 0;
            cloakTimer = 0;
            bar.curFrame = 0;
        }
    }
}

// Sets up the status bar and spells in the bottom left
void initStatus() {

    bar.screenRow = 145;
    bar.screenCol = 20;
    bar.curFrame = 0;

    cheat.screenRow = bar.screenRow - 4;
    cheat.screenCol = bar.screenCol;
    cheat.hide = 1;

    for (int i = 0; i < SPELLCOUNT; i++) {

        spellStatus[i].screenRow = 142;
        spellStatus[i].screenCol = 52 + (10 * i);
        spellStatus[i].hide = 0;
    }
}

// Handles every-frame changes for the status bar and spells
void updateStatus() {

    // If cloak on, decrease green bar
    int time;
    if (cheatOn) {
        time = SUPERCLOAKFRAMES;
        cheat.hide = 0;
    } else {
        time = CLOAKFRAMES;
    }
    if (cloakOn && cloakTimer > ((time / 4) * (bar.curFrame + 1))) {
        bar.curFrame++;
    }

    // If charging, increase red bar
    int chargeTime;
    if (cheatOn) {
        chargeTime = SUPERCLOAKRECHARGE;
    } else {
        chargeTime = CLOAKRECHARGE;
    }
    if (recharging && cloakTimer > ((chargeTime / 4) * (5 - bar.curFrame))) {
        bar.curFrame--;
    }

    // Hide spell icons as used, max 3 shown
    if (spellsRemaining < SPELLCOUNT) {
        spellStatus[spellsRemaining].hide = 1;
    }
}

// Draws the cloak and status bar
void drawStatus() {

    // Draw cloak
    shadowOAM[3].attr0 = (ROWMASK & (bar.screenRow - 4)) | ATTR0_SQUARE;
    shadowOAM[3].attr1 = (COLMASK & (bar.screenCol - 17)) | ATTR1_SMALL;
    shadowOAM[3].attr2 = ATTR2_PALROW(4) | ATTR2_TILEID(0, 18);

    // Draw status bar
    shadowOAM[4].attr0 = (ROWMASK & bar.screenRow) | ATTR0_WIDE;
    shadowOAM[4].attr1 = (COLMASK & bar.screenCol) | ATTR1_SMALL;
    if (recharging) {
        shadowOAM[4].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(bar.curFrame, 20);
    } else {
        shadowOAM[4].attr2 = ATTR2_PALROW(4) | ATTR2_TILEID(bar.curFrame, 20);
    }

    // Draw cheat indicator
    if (cheat.hide) {
        shadowOAM[5].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[5].attr0 = (ROWMASK & cheat.screenRow) | ATTR0_WIDE;
        shadowOAM[5].attr1 = (COLMASK & cheat.screenCol) | ATTR1_MEDIUM;
        shadowOAM[5].attr2 = ATTR2_PALROW(4) | ATTR2_TILEID(5, 20);
    }

    // Draw spellStatus
    for (int i = 0; i < SPELLCOUNT; i++) {

        if (spellStatus[i].hide) {
            shadowOAM[9 + i].attr0 |= ATTR0_HIDE;
        } else {
            shadowOAM[9 + i].attr0 = (ROWMASK & spellStatus[i].screenRow) | ATTR0_SQUARE;
            shadowOAM[9 + i].attr1 = (COLMASK & spellStatus[i].screenCol) | ATTR1_SMALL;
            shadowOAM[9 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(8, 24);
        }
    }
}

// Sets up the player lives
void initLives() {

    for (int i = 0; i < LIVES; i++) {
        lives[i].screenRow = 2;
        lives[i].screenCol = (i * 19) + 5;
        lives[i].hide = 0;
    }
}

// Updates the player lives
void updateLives() {

    for (int i = 0; i < LIVES; i++) {

        // Show only the remaining lives
        if (i < livesRemaining) {
            lives[i].hide = 0;
        } else {
            lives[i].hide = 1;
        }
    }
}

// Draws the remaining lives, represented by hearts
void drawLives() {

    for (int i = 0; i < LIVES; i++) {

        if (lives[i].hide) {
            shadowOAM[6 + i].attr0 |= ATTR0_HIDE;
        } else {
            shadowOAM[6 + i].attr0 = (ROWMASK & lives[i].screenRow) | ATTR0_SQUARE;
            shadowOAM[6 + i].attr1 = (COLMASK & lives[i].screenCol) | ATTR1_SMALL;
            shadowOAM[6 + i].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(0, 16);
        }
    }
}
