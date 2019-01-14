#include <stdlib.h>
#include "myLib.h"
#include "movement.h"
#include "potionsCollision.h"
#include "corridorsCollision.h"
#include "forestCollision.h"

// Variables
int stuckCounter;

// Returns whether or not the anisprite can currently move up
int canMoveUp(ANISPRITE* a) {

    // Level 1
    if (level == LEVEL1) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow - a->rdel >= 0
            && potionsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + a->width - 1, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + (a->width / 2), mapWidth)];
    }

    // Level 2
    if (level == LEVEL2) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow - a->rdel >= 0
            && corridorsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + a->width - 1, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + (a->width / 2), mapWidth)];
    }

    // Level 3
    if (level == LEVEL3) {

    // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow - a->rdel >= 0
            && forestCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + a->width - 1, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow - a->rdel, a->worldCol + (a->width / 2), mapWidth)];
    }

    return 0;
}

// Returns whether or not the anisprite can currently move down
int canMoveDown(ANISPRITE* a) {

    // Level 1
    if (level == LEVEL1) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow + a->height + a->rdel - 1 < mapHeight
            && potionsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + a->width - 1, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + (a->width / 2), mapWidth)];
    }

    // Level 2
    if (level == LEVEL2) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow + a->height + a->rdel - 1 < mapHeight
            && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + a->width - 1, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + (a->width / 2), mapWidth)];
    }

    // Level 3
    if (level == LEVEL3) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldRow + a->height + a->rdel - 1 < mapHeight
            && forestCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + a->width - 1, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + a->height + a->rdel - 1, a->worldCol + (a->width / 2), mapWidth)];
    }

    return 0;
}

// Returns wheter or not the anisprite can currently move left
int canMoveLeft(ANISPRITE* a) {

    // Level 1
    if (level == LEVEL1) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldCol - a->cdel >= 0
            && potionsCollisionBitmap[OFFSET(a->worldRow, a->worldCol - a->cdel, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol - a->cdel, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol - a->cdel, mapWidth)];
    }

    // Level 2
    if (level == LEVEL2) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        // Also check to make sure remaining in room
        return a->worldCol - a->cdel >= 0
            && corridorsCollisionBitmap[OFFSET(a->worldRow, a->worldCol - a->cdel, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol - a->cdel, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol - a->cdel, mapWidth)];
    }

    // Level 3
    if (level == LEVEL3) {

        // Make sure anisprite doesn't go off the map, check collision map as well
        return a->worldCol - a->cdel >= 0
            && forestCollisionBitmap[OFFSET(a->worldRow, a->worldCol - a->cdel, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol - a->cdel, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol - a->cdel, mapWidth)];
    }

    return 0;
}

// Used for prefect sprites in level 2 to keep them from leaving the room
int prefectCanMoveLeft(ANISPRITE* a) {

    return a->worldCol > 180
        && corridorsCollisionBitmap[OFFSET(a->worldRow - 1, a->worldCol - a->cdel, mapWidth)]
        && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height - 2, a->worldCol - a->cdel, mapWidth)]
        && corridorsCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol - a->cdel, mapWidth)];
}

// Returns whether or not the anisprite can currently move right
int canMoveRight(ANISPRITE* a) {

    // Level 1
    if (level == LEVEL1) {

        // make sure anisprite doesn't go off the map, check collision map as well
        return a->worldCol + a->width + a->cdel - 1 < mapWidth
            && potionsCollisionBitmap[OFFSET(a->worldRow, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && potionsCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol + a->width + a->cdel - 1, mapWidth)];
    }

    // Level 2
    if (level == LEVEL2) {

        // make sure anisprite doesn't go off the map, check collision map as well
        return a->worldCol + a->width + a->cdel - 1 < mapWidth
            && corridorsCollisionBitmap[OFFSET(a->worldRow, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && corridorsCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol + a->width + a->cdel - 1, mapWidth)];
    }

    // Level 3
    if (level == LEVEL3) {

        // make sure anisprite doesn't go off the map, check collision map as well
        return a->worldCol + a->width + a->cdel - 1 < mapWidth
            && forestCollisionBitmap[OFFSET(a->worldRow, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + a->height - 1, a->worldCol + a->width + a->cdel - 1, mapWidth)]
            && forestCollisionBitmap[OFFSET(a->worldRow + (a->height / 2), a->worldCol + a->width + a->cdel - 1, mapWidth)];
    }

    return 0;
}

// Causes given anisprite to follow the player around the screen
void followPlayer(ANISPRITE* a) {

    // If Harry is above anisprite, move up
    if (harry.worldRow < a->worldRow && canMoveUp(a)) {
        a->worldRow -= a->rdel;
        a->aniState = BACK;
    }

    // If Harry is below anisprite, move down
    if (harry.worldRow > a->worldRow && canMoveDown(a)) {
        a->worldRow += a->rdel;
        a->aniState = FRONT;
    }

    // If Harry is to the left of anisprite, move left
    // If Level 2, keep prefects in room
    if (level == LEVEL2) {

        if (harry.worldCol < a->worldCol && prefectCanMoveLeft(a)) {
            a->worldCol -= a->cdel;
            a->aniState = LEFT;
        }
    } else {

        if (harry.worldCol < a->worldCol && canMoveLeft(a)) {
            a->worldCol -= a->cdel;
            a->aniState = LEFT;
        }
    }

    // If Harry is to the right of anisprite, move right
    if (harry.worldCol > a->worldCol && canMoveRight(a)) {
        a->worldCol += a->cdel;
        a->aniState = RIGHT;
    }

    // Reset moveCounter for random movement
    a->moveCounter = CHANGEDIR;
}

// Returns whether or not the anisprite is close to the player
int closeToPlayer(ANISPRITE* a) {

    // Difference in rows of the two sprites
    int rowDif = harry.worldRow - a->worldRow;

    // Difference in cols of the two sprites
    int colDif = harry.worldCol - a->worldCol;

    // Square of the distance between the two sprites
    int dist = (rowDif * rowDif) + (colDif * colDif);

    return (dist < (CLOSEDISTANCE * CLOSEDISTANCE));
}

// Causes given anisprite to move in random directions
void moveRandomly(ANISPRITE* a) {

    // If Level 2, keep prefects in room
    if (level == LEVEL2) {

        // Change direction if moveCounter is greater than frames in CHANGEDIR
        if (a->moveCounter > CHANGEDIR) {

            // Fixes glitch where sprite could rapidly change btwn states when stuck
            // Only tries 4 times to fix, then moves on with program
            int newState = rand() % 4;
            while ((stuckCounter < 4) && ((newState == FRONT && !canMoveDown(a)) || (newState == BACK && !canMoveUp(a))
                    || (newState == LEFT && !prefectCanMoveLeft(a)) || (newState == RIGHT && !canMoveRight(a)))) {
                newState = rand() % 4;
                stuckCounter++;
            }

            a->aniState = newState;
            a->moveCounter = 0;
            stuckCounter = 0;
        }

        // Check for running into barrier
        if ((a->aniState == FRONT && !canMoveDown(a)) || (a->aniState == BACK && !canMoveUp(a))
                || (a->aniState == LEFT && !prefectCanMoveLeft(a)) || (a->aniState == RIGHT && !canMoveRight(a))) {
            a->moveCounter = CHANGEDIR + 1;
        }
    } else {  // Not level 2

        // Change direction if moveCounter is greater than frames in CHANGEDIR
        if (a->moveCounter > CHANGEDIR) {

            // Fixes glitch where sprite could rapidly change btwn states when stuck
            // Only tries 4 times to fix, then moves on with program
            int newState = rand() % 4;
            while ((stuckCounter < 4) && ((newState == FRONT && !canMoveDown(a)) || (newState == BACK && !canMoveUp(a))
                    || (newState == LEFT && !canMoveLeft(a)) || (newState == RIGHT && !canMoveRight(a)))) {
                newState = rand() % 4;
                stuckCounter++;
            }

            a->aniState = newState;
            a->moveCounter = 0;
            stuckCounter = 0;
        }

        // Check for running into barrier
        if ((a->aniState == FRONT && !canMoveDown(a)) || (a->aniState == BACK && !canMoveUp(a))
                || (a->aniState == LEFT && !canMoveLeft(a)) || (a->aniState == RIGHT && !canMoveRight(a))) {
            a->moveCounter = CHANGEDIR + 1;
        }
    }

    if (a->aniState == FRONT) {
        a->worldRow += a->rdel;
    }
    if (a->aniState == BACK) {
        a->worldRow -= a->rdel;
    }
    if (a->aniState == LEFT) {
        a->worldCol -= a->cdel;
    }
    if (a->aniState == RIGHT) {
        a->worldCol += a->cdel;
    }

    a->moveCounter++;
}

// For level 2, returns whether or not sprites are in the large room
int inRoom(ANISPRITE* a) {

    return (a->worldRow < 220) && (a->worldCol > 180);
}
