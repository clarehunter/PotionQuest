// Item Struct
typedef struct {
    int screenRow;
    int screenCol;
    int worldRow;
    int worldCol;
    int width;
    int height;
    int hide;
    int collected;
} ITEM;

// Constants
#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define ITEMCOUNT 3

// Variables
extern int hOff;
extern int vOff;
extern int hOffRiver;
extern int mapHeight;
extern int mapWidth;
extern OBJ_ATTR shadowOAM[128];
extern int level;
extern ANISPRITE harry;
extern ITEM items[ITEMCOUNT];
extern ITEM collection[ITEMCOUNT];
extern ITEM exitLevel;
extern int switchLevels;
extern int resetLevel;
extern int gameOver;

// Prototypes
void initGame();
void initLevel1();
void initLevel2();
void initLevel3();

void updateGame();
void drawGame();

void initItems();
void updateItems();
void drawItems();

void initCollection();
void updateCollection();
void drawCollection();

void initExit();
void updateExit();
void drawExit();
