#include <stdlib.h>
#include "myLib.h"
#include "enemy.h"
#include "movement.h"

// Variables
ANISPRITE snape;
ANISPRITE prefects[PREFECTCOUNT];
ANISPRITE spiders[SPIDERCOUNT];
ANISPRITE ghosts[GHOSTCOUNT];
int counter;

// Set up the enemies
void initEnemies() {

    // Level 1
    if (level == LEVEL1) {

        initSnape();

        // Hide enemies from other levels
        for (int i = 0; i < PREFECTCOUNT; i++) {
            prefects[i].hide = 1;
        }
        for (int i = 0; i < GHOSTCOUNT; i++) {
            ghosts[i].hide = 1;
        }
        for (int i = 0; i < SPIDERCOUNT; i++) {
            spiders[i].hide = 1;
        }

        counter = 0;
    }

    // Level 2
    if (level == LEVEL2) {

        initPrefects();
        initGhosts();

        // Hide enemies from other levels
        snape.hide = 1;
        for (int i = 0; i < SPIDERCOUNT; i++) {
            spiders[i].hide = 1;
        }

        counter = 0;
    }

    // Level 3
    if (level == LEVEL2) {

        initSpiders();

        // Hide enemies from other levels
        snape.hide = 1;
        for (int i = 0; i < PREFECTCOUNT; i++) {
            prefects[i].hide = 1;
        }
        for (int i = 0; i < GHOSTCOUNT; i++) {
            ghosts[i].hide = 1;
        }

        counter = 0;
    }
}

// Handle every-frame actions of the enemies
void updateEnemies() {

    // Level 1
    if (level == LEVEL1) {
        updateSnape();
    }

    // Level 2
    if (level == LEVEL2) {
        updatePrefects();
        updateGhosts();
    }

    // Level 3
    if (level == LEVEL3) {
        updateSpiders();
    }

    animateEnemies();
    checkCaught();

    counter++;
}

// Handle enemy animation states
void animateEnemies() {

    // Level 1
    if (level == LEVEL1) {
        animatePerson(&snape);
    }

    // Level 2
    if (level == LEVEL2) {

        for (int i = 0; i < PREFECTCOUNT; i++) {
            animatePerson(&prefects[i]);
        }
    }

    // Level 3
    if (level == LEVEL3) {

        for (int i = 0; i < SPIDERCOUNT; i++) {
            animateSpider(&spiders[i]);
        }
    }

}

// Draw the enemies
void drawEnemies() {

    // Level 1: Draw Snape
    if (level == LEVEL1) {

        if (snape.hide) {
            shadowOAM[16].attr0 |= ATTR0_HIDE;
        } else {
            shadowOAM[16].attr0 = (ROWMASK & snape.screenRow) | ATTR0_SQUARE;
            shadowOAM[16].attr1 = (COLMASK & snape.screenCol) | ATTR1_MEDIUM;
            shadowOAM[16].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID((snape.aniState + 4) * 4, snape.curFrame * 4);
        }
        shadowOAM[17].attr0 |= ATTR0_HIDE;
        shadowOAM[18].attr0 |= ATTR0_HIDE;
        shadowOAM[19].attr0 |= ATTR0_HIDE;
        shadowOAM[20].attr0 |= ATTR0_HIDE;
        shadowOAM[21].attr0 |= ATTR0_HIDE;
    }

    // Level 2: Draw prefects and ghosts
    if (level == LEVEL2) {

        for (int i = 0; i < PREFECTCOUNT; i++) {

            if (prefects[i].hide) {
                shadowOAM[16 + i].attr0 |= ATTR0_HIDE;
            } else {
                shadowOAM[16 + i].attr0 = (ROWMASK & prefects[i].screenRow) | ATTR0_SQUARE;
                shadowOAM[16 + i].attr1 = (COLMASK & prefects[i].screenCol) | ATTR1_MEDIUM;
                shadowOAM[16 + i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID((prefects[i].aniState + 4) * 4, (prefects[i].curFrame + (3 * i)) * 4);
            }
        }
        for (int i = 0; i < GHOSTCOUNT; i++) {

            if (ghosts[i].hide) {
                shadowOAM[18 + i].attr0 |= ATTR0_HIDE;
            } else {
                shadowOAM[18 + i].attr0 = (ROWMASK & ghosts[i].screenRow) | ATTR0_SQUARE | ATTR0_BLEND;
                shadowOAM[18 + i].attr1 = (COLMASK & ghosts[i].screenCol) | ATTR1_MEDIUM;
                shadowOAM[18 + i].attr2 = ATTR2_PALROW(6) | ATTR2_TILEID(ghosts[i].aniState * 4, 12);
            }
        }
    }

    // Level 3: Draw spiders
    if (level == LEVEL3) {

        for (int i = 0; i < SPIDERCOUNT; i++) {

            if (spiders[i].hide) {
                shadowOAM[16 + i].attr0 |= ATTR0_HIDE;
            } else {
                shadowOAM[16 + i].attr0 = (ROWMASK & spiders[i].screenRow) | ATTR0_SQUARE;
                shadowOAM[16 + i].attr1 = (COLMASK & spiders[i].screenCol) | ATTR1_MEDIUM;
                shadowOAM[16 + i].attr2 = ATTR2_PALROW(1) | ATTR2_TILEID((spiders[i].aniState + 4) * 4, spiders[i].curFrame * 4);
            }
        }
        shadowOAM[19].attr0 |= ATTR0_HIDE;
        shadowOAM[20].attr0 |= ATTR0_HIDE;
        shadowOAM[21].attr0 |= ATTR0_HIDE;
    }
}

// Set up Snape
void initSnape() {

    snape.width = 27;
    snape.height = 32;
    snape.rdel = 1;
    snape.cdel = 1;

    // Place in chosen world location
    snape.worldRow = 190;
    snape.worldCol = 35;
    snape.aniCounter = 0;
    snape.curFrame = 1;
    snape.numFrames = 3;
    snape.aniState = FRONT;
    snape.hide = 0;
    snape.stunned = 0;
    snape.stunCounter = 0;
    snape.moveCounter = 0;
}

// Handle every-frame actions for snape
void updateSnape() {

    // While stunned, count frames until time for spell to wear off
    if (snape.stunned) {

        snape.stunCounter += 1;

        if (snape.stunCounter > STUNFRAMES) {
            snape.stunned = 0;
            snape.stunCounter = 0;
        }
    }

    // If not stunned, move every other frame
    if (!snape.stunned && counter % 2 == 0) {

        // If close to player and cloak is off, follow player
        if (closeToPlayer(&snape) && !cloakOn) {

            followPlayer(&snape);
        } else {
            // Else move randomly
            moveRandomly(&snape);
        }
    }

    // Check for stun
    checkStun(&snape);

    // Update screen row and screen col
    snape.screenRow = snape.worldRow - vOff;
    snape.screenCol = snape.worldCol - hOff;

    // Hide when not on screen to avoid display errors
    if (snape.screenRow + snape.height < 0 || snape.screenRow > 160
            || snape.screenCol + snape.width < 0 || snape.screenCol > 240) {
        snape.hide = 1;
    } else {
        snape.hide = 0;
    }
}


// Set up the prefects
void initPrefects() {

    for (int i = 0; i < PREFECTCOUNT; i++) {

        prefects[i].width = 32;
        prefects[i].height = 32;
        prefects[i].rdel = 1;
        prefects[i].cdel = 1;
        prefects[i].aniCounter = 0;
        prefects[i].curFrame = 1;
        prefects[i].numFrames = 3;
        prefects[i].aniState = FRONT;
        prefects[i].hide = 0;
        prefects[i].stunned = 0;
        prefects[i].stunCounter = 0;
        prefects[i].moveCounter = 0;
    }

    // Place in chosen world locations
    prefects[0].worldRow = 50;
    prefects[0].worldCol = 250;
    prefects[1].worldRow = 100;
    prefects[1].worldCol = 400;
}

// Handle every-frame actions of the prefects
void updatePrefects() {

    for (int i = 0; i < PREFECTCOUNT; i++) {

        // Make width more accurate
        if (prefects[i].aniState == FRONT) {
            prefects[i].width = 27;
        } else if (prefects[i].aniState == LEFT) {
            prefects[i].width = 25;
        } else if (prefects[i].aniState == RIGHT) {
            prefects[i].width = 26;
        } else if (prefects[i].aniState == BACK) {
            prefects[i].width = 25;
        }

        // While stunned, count frames until time for spell to wear off
        if (prefects[i].stunned) {

            prefects[i].stunCounter++;

            if (prefects[i].stunCounter > STUNFRAMES) {
                prefects[i].stunned = 0;
                prefects[i].stunCounter = 0;
            }
        }

        // If not stunned, move every other frame
        if (!prefects[i].stunned && counter % 2 == 0) {

            // If close to player, cloak is off, and player is in large room,
            // follow player
            if (inRoom(&harry) && closeToPlayer(&prefects[i]) && !cloakOn) {

                followPlayer(&prefects[i]);
            } else {
                // Else move randomly
                moveRandomly(&prefects[i]);
            }
        }

        // Check for stun
        checkStun(&prefects[i]);

        // Update screen row and screen col
        prefects[i].screenRow = prefects[i].worldRow - vOff;
        prefects[i].screenCol = prefects[i].worldCol - hOff;

        // Hide when not on screen to avoid display errors
        if (prefects[i].screenRow + prefects[i].height < 0 || prefects[i].screenRow > 160
                || prefects[i].screenCol + prefects[i].width < 0 || prefects[i].screenCol > 240) {
            prefects[i].hide = 1;
        } else {
            prefects[i].hide = 0;
        }
    }
}

// Set up the ghosts
void initGhosts() {

    for (int i = 0; i < GHOSTCOUNT; i++) {

        ghosts[i].width = 32;
        ghosts[i].height = 32;
        ghosts[i].rdel = 1;
        ghosts[i].cdel = 1;
        ghosts[i].hide = 0;
        ghosts[i].stunned = 0;
        ghosts[i].stunCounter = 0;
        ghosts[i].moveCounter = 0;
    }

    // Set direction
    ghosts[0].aniState = RIGHT;
    ghosts[1].aniState = FRONT;;
    ghosts[2].aniState = FRONT;
    ghosts[3].aniState = BACK;

    // Place in chosen world locations
    ghosts[0].worldRow = 282;
    ghosts[0].worldCol = 1;
    ghosts[1].worldRow = 250;
    ghosts[1].worldCol = 248;
    ghosts[2].worldRow = 400;
    ghosts[2].worldCol = 160;
    ghosts[3].worldRow = 478;
    ghosts[3].worldCol = 336;
}

// Handle every-frame actions of the ghosts
void updateGhosts() {

    // Ghost 0
    if (ghosts[0].stunned)  {

        ghosts[0].stunCounter++;

        if (ghosts[0].stunCounter > STUNFRAMES) {
            ghosts[0].stunned = 0;
            ghosts[0].stunCounter = 0;
        }
    } else if (counter % 2 == 0 && ghosts[0].moveCounter <= 0) {
        // not stunned, walk back and forth moving every other frame
        // only move if past the pause time at each end of path

        if (ghosts[0].aniState == RIGHT) {  // moving right
            ghosts[0].worldCol += ghosts[0].cdel;

            if (ghosts[0].worldCol > 80) {
                ghosts[0].aniState = LEFT;
                ghosts[0].moveCounter = 40;
            }
        } else {  // moving left
            ghosts[0].worldCol -= ghosts[0].cdel;

            if (ghosts[0].worldCol < 1) {
                ghosts[0].aniState = RIGHT;
                ghosts[0].moveCounter = 40;
            }
        }
    }
    if (ghosts[0].moveCounter > 0) {
        ghosts[0].moveCounter--;
    }

    // Ghost 1
    if (ghosts[1].stunned)  {

        ghosts[1].stunCounter++;

        if (ghosts[1].stunCounter > STUNFRAMES) {
            ghosts[1].stunned = 0;
            ghosts[1].stunCounter = 0;
        }
    } else if (counter % 2 == 0) {  // not stunned, walk back and forth
        // moving every other frame

        if (ghosts[1].aniState == FRONT) {  // moving down
            ghosts[1].worldRow += ghosts[1].rdel;

            if (ghosts[1].worldRow > 478) {
                ghosts[1].aniState = BACK;
            }
        } else {  // moving up
            ghosts[1].worldRow -= ghosts[1].rdel;

            if (ghosts[1].worldRow < 250) {
                ghosts[1].aniState = FRONT;
            }
        }
    }

    // Ghost 2
    if (ghosts[2].stunned)  {

        ghosts[2].stunCounter++;

        if (ghosts[2].stunCounter > STUNFRAMES) {
            ghosts[2].stunned = 0;
            ghosts[2].stunCounter = 0;
        }
    } else if (counter % 2 == 0) {  // not stunned, walk back and forth
        // moving every other frame

        if (ghosts[2].aniState == FRONT) {  // moving down
            ghosts[2].worldRow += ghosts[2].rdel;

            if (ghosts[2].worldRow > 478) {
                ghosts[2].aniState = BACK;
            }
        } else {  // moving up
            ghosts[2].worldRow -= ghosts[2].rdel;

            if (ghosts[2].worldRow < 400) {
                ghosts[2].aniState = FRONT;
            }
        }
    }

    // Ghost 3
    if (ghosts[3].stunned)  {

        ghosts[3].stunCounter++;

        if (ghosts[3].stunCounter > STUNFRAMES) {
            ghosts[3].stunned = 0;
            ghosts[3].stunCounter = 0;
        }
    } else if (counter % 2 == 0) {  // not stunned, walk back and forth
        // moving every other frame

        if (ghosts[3].aniState == FRONT) {  // moving down
            ghosts[3].worldRow += ghosts[3].rdel;

            if (ghosts[3].worldRow > 478) {
                ghosts[3].aniState = BACK;
            }
        } else {  // moving up
            ghosts[3].worldRow -= ghosts[3].rdel;

            if (ghosts[3].worldRow < 400) {
                ghosts[3].aniState = FRONT;
            }
        }
    }

    // Updates for all ghosts
    for (int i = 0; i < GHOSTCOUNT; i++) {

        // Check for stun
        checkStun(&ghosts[i]);

        // Make width and height more accurate
        if (ghosts[i].aniState == FRONT) {
            ghosts[i].width = 27;
            ghosts[i].height = 32;
        } else if (ghosts[i].aniState == LEFT) {
            ghosts[i].width = 21;
            ghosts[i].height = 32;
        } else if (ghosts[i].aniState == RIGHT) {
            ghosts[i].width = 21;
            ghosts[i].height = 32;
        } else if (ghosts[i].aniState == BACK) {
            ghosts[i].width = 25;
            ghosts[i].height = 31;
        }

        // Update screen row and screen col
        ghosts[i].screenRow = ghosts[i].worldRow - vOff;
        ghosts[i].screenCol = ghosts[i].worldCol - hOff;

        // Hide when stunned or not on screen
        if (ghosts[i].screenRow + ghosts[i].height < 0 || ghosts[i].screenRow > 160
                || ghosts[i].screenCol + ghosts[i].width < 0 || ghosts[i].screenCol > 240
                || ghosts[i].stunned) {
            ghosts[i].hide = 1;
        } else {
            ghosts[i].hide = 0;
        }
    }
}

// Set up the spiders
void initSpiders() {

    for (int i = 0; i < SPIDERCOUNT; i++) {

        spiders[i].width = 32;
        spiders[i].height = 32;
        spiders[i].rdel = 1;
        spiders[i].cdel = 1;
        spiders[i].aniCounter = i;  // Move legs at different times
        spiders[i].curFrame = 7;
        spiders[i].numFrames = 8;
        spiders[i].aniState = FRONT;
        spiders[i].hide = 0;
        spiders[i].stunned = 0;
        spiders[i].stunCounter = 0;
        spiders[i].moveCounter = 0;
    }

    // Place in chosen world locations
    spiders[0].worldRow = 300;
    spiders[0].worldCol = 100;
    spiders[1].worldRow = 100;
    spiders[1].worldCol = 400;
    spiders[2].worldRow = 100;
    spiders[2].worldCol = 250;
}

// Handle every-frame actions of the spiders
void updateSpiders() {

    for (int i = 0; i < SPIDERCOUNT; i++) {

        // Make width and height more accurate
        if (spiders[i].aniState == FRONT) {
            spiders[i].width = 32;
            spiders[i].height = 29;
        } else if (spiders[i].aniState == LEFT) {
            spiders[i].width = 29;
            spiders[i].height = 32;
        } else if (spiders[i].aniState == RIGHT) {
            spiders[i].width = 29;
            spiders[i].height = 32;
        } else if (spiders[i].aniState == BACK) {
            spiders[i].width = 32;
            spiders[i].height = 29;
        }

        // While stunned, count frames until time for spell to wear off
        if (spiders[i].stunned) {

            spiders[i].stunCounter++;

            if (spiders[i].stunCounter > STUNFRAMES) {
                spiders[i].stunned = 0;
                spiders[i].stunCounter = 0;
            }
        }

        // If not stunned, move every 2 frames
        if (!spiders[i].stunned && counter % 2 == 0) {

            // If close to player and cloak is off, follow player
            if (closeToPlayer(&spiders[i]) && !cloakOn) {

                followPlayer(&spiders[i]);
            } else {
                // Else move randomly
                moveRandomly(&spiders[i]);
            }
        }

        // Check for stun
        checkStun(&spiders[i]);

        // Update screen row and screen col
        spiders[i].screenRow = spiders[i].worldRow - vOff;
        spiders[i].screenCol = spiders[i].worldCol - hOff;

        // Hide when not on screen to avoid display errors
        if (spiders[i].screenRow + spiders[i].height < 0 || spiders[i].screenRow > 160
                || spiders[i].screenCol + spiders[i].width < 0 || spiders[i].screenCol > 240) {
            spiders[i].hide = 1;
        } else {
            spiders[i].hide = 0;
        }
    }
}

// Handles animations for a person sprite
void animatePerson(ANISPRITE* p) {

    // Change the animation frame every 20 frames of gameplay
    if(p->aniCounter % 20 == 0) {
        p->curFrame = (p->curFrame + 1) % p->numFrames;
    }

    // If the person is stunned, frame is standing
    if (p->stunned) {
        p->curFrame = 1;
        p->aniCounter = 0;
    } else {
        p->aniCounter++;
    }
}

// Handles animations for a spider sprite
void animateSpider(ANISPRITE* s) {

    int prevFrame = s->curFrame;

     // Change the animation frame every 4 frames of gameplay
    if(s->aniCounter % 4 == 0) {
        s->curFrame = (s->curFrame + 1) % s->numFrames;
    }

    // If the spider is stunned or tricked by cloak, frame is same as previous
    if (s->stunned) {
        s->curFrame = prevFrame;
        s->aniCounter = 0;
    } else {
        s->aniCounter++;
    }
}

// Checks to see if the given enemy has been stunned
void checkStun(ANISPRITE* a) {

    // If spell hits enemy, hide spell and stun enemy
    for (int i = 0; i < SPELLARSENAL; i++) {

        if (stupefy[i].active && collision(stupefy[i].worldRow, stupefy[i].worldCol, stupefy[i].height,
                stupefy[i].width, a->worldRow, a->worldCol, a->height, a->width)) {
            stupefy[i].active = 0;
            a->stunned = 1;
        }
    }
}

// Checks for collisions with harry and enemies to see if game is over
void checkCaught() {

    // Level 1
    if (level == LEVEL1) {

        // Snape
        if (collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                snape.worldRow, snape.worldCol, snape.height, snape.width)) {

            livesRemaining--;

            if (!livesRemaining) {
                gameOver = 1;
            } else {
                resetLevel = 1;
            }
        }
    }

    // Level 2
    if (level == LEVEL2) {

        // Prefects
        for (int i = 0; i < PREFECTCOUNT; i++) {
            if (collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                    prefects[i].worldRow, prefects[i].worldCol, prefects[i].height, prefects[i].width)) {

                livesRemaining--;

                if (!livesRemaining) {
                    gameOver = 1;
                } else {
                    resetLevel = 1;
                }
            }
        }

        // Ghosts
        for (int i = 0; i < GHOSTCOUNT; i++) {
            if (!ghosts[i].stunned && collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                    ghosts[i].worldRow, ghosts[i].worldCol, ghosts[i].height, ghosts[i].width)) {

                livesRemaining--;

                if (!livesRemaining) {
                    gameOver = 1;
                } else {
                    resetLevel = 1;
                }
            }
        }
    }

    // Level 3
    if (level == LEVEL3) {

        // Spiders
        for (int i = 0; i < SPIDERCOUNT; i++) {
            if (collision(harry.worldRow, harry.worldCol, harry.height, harry.width,
                    spiders[i].worldRow, spiders[i].worldCol, spiders[i].height, spiders[i].width)) {

                livesRemaining--;

                if (!livesRemaining) {
                    gameOver = 1;
                } else {
                    resetLevel = 1;
                }
            }
        }
    }
}
