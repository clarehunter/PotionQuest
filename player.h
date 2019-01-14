// Status Bar/Spells Struct
typedef struct {
    int screenRow;
    int screenCol;
    int curFrame;
    int hide;
} STATUS;

// Constants
#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define SPELLARSENAL 3  // Also defined in enemy.h, number of spells in array
#define SPELLCOUNT 3  // number of spells that can be shot per level, max 6
#define CLOAKFRAMES 400
#define SUPERCLOAKFRAMES 1400
#define CLOAKRECHARGE 600
#define SUPERCLOAKRECHARGE 140
#define LIVES 3

// Variables
extern int hOff;
extern int vOff;
extern int hOffRiver;
extern int mapHeight;
extern int mapWidth;
extern OBJ_ATTR shadowOAM[128];
extern ANISPRITE harry;
extern int level;
extern SPELL stupefy[SPELLARSENAL];
extern int spellsRemaining;
extern int spellTimer;
extern int cloakOn;
extern int cloakTimer;
extern int recharging;
extern STATUS lives[LIVES];
extern int livesRemaining;
extern STATUS bar;
extern STATUS cheat;
extern STATUS spellStatus[SPELLCOUNT];
extern int cheatOn;

// Prototypes
void initPlayer();
void updatePlayer();
void animatePlayer();
void drawPlayer();

void initSpells();
void castSpell();
void updateSpells();
void animateSpells();
void drawSpells();

void initCloak();
void cloak();
void updateCloak();

void initStatus();
void updateStatus();
void drawStatus();

void initLives();
void updateLives();
void drawLives();
