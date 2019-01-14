// Constants
#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define CLOSEDISTANCE 120
#define CHANGEDIR 200

// Variables
extern int hOff;
extern int vOff;
extern int mapHeight;
extern int mapWidth;
extern int level;
extern ANISPRITE harry;
extern int stuckCounter;

// Prototypes
int canMoveUp(ANISPRITE* a);
int canMoveDown(ANISPRITE* a);
int canMoveLeft(ANISPRITE* a);
int prefectCanMoveLeft(ANISPRITE* a);
int canMoveRight(ANISPRITE* a);
void followPlayer(ANISPRITE* a);
int closeToPlayer(ANISPRITE* a);
void moveRandomly(ANISPRITE* a);
int inRoom(ANISPRITE* a);
