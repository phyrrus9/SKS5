#include "engine.h"

/* Global variables */
const char *title = "Super Key Seeker 5: The lost dimension";
const char *title2 = "AUX";
GLfloat Yangle = 0.0f, Zangle = 0.0f;
bool jumping = 0;
int mouseX, mouseY;
bool gameStarted = false, gameOver = false;
char levelNames[LEVELS][128] =
{
	"C:\\Users\\phyrrus9\\Desktop\\map.map"
};
int requiredKeys;
float telr = 0, telg = 0, telb = 1;
unsigned long frameCounter = 0;
float frameRate = 0;
clock_t frameTime = clock();
vector<bulletType> bullets;
vector<zombieType *> zombies;
vector<char *> debugbuf;
bool debugVisible = false;
/*env start*/
viewType pl_view = V_NORTH;
int pl_xpos = 1, pl_ypos = 1, pl_zpos = 1;
int pl_keys = 0, pl_coins = 0, pl_walls = 1, pl_lvl = 0, pl_zkills = 0, pl_health = 100;
bool pl_hulk_smash = false;
/*env end*/
