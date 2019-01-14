// Select Struct
typedef struct {
    int screenRow;
    int screenCol;
    int choice;
} SELECT;

// Selection Choice Enums
enum {START, INSTR};
enum {RESUME, QUIT};
enum {PLAY, MENU};

// Variables
extern SELECT wand;

// Prototypes
void initSplash();
void updateSplash();
void drawSplash();

void initInstructions();
void updateInstructions();
void drawInstructions();

void initPause();
void updatePause();
void drawPause();

void drawWand();
