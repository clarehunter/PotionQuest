// Constants
#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define SPELLARSENAL 3  // Also defined in player.h
#define STUNFRAMES 300
#define PREFECTCOUNT 2
#define SPIDERCOUNT 3
#define GHOSTCOUNT 4

// Variables
extern int hOff;
extern int vOff;
extern int mapHeight;
extern int mapWidth;
extern OBJ_ATTR shadowOAM[128];
extern ANISPRITE harry;
extern ANISPRITE snape;
extern ANISPRITE prefects[PREFECTCOUNT];
extern ANISPRITE ghosts[GHOSTCOUNT];
extern ANISPRITE spiders[SPIDERCOUNT];
extern SPELL stupefy[SPELLARSENAL];
extern int level;
extern int cloakOn;
extern int counter;
extern int gameOver;
extern int resetLevel;
extern int livesRemaining;

// Prototypes
void initEnemies();
void updateEnemies();
void animateEnemies();
void drawEnemies();

void initSnape();
void updateSnape();

void initPrefects();
void updatePrefects();

void initGhosts();
void updateGhosts();

void initSpiders();
void updateSpiders();

void animatePerson(ANISPRITE* p);
void animateSpider(ANISPRITE* s);

void checkStun(ANISPRITE* a);

void checkCaught();
