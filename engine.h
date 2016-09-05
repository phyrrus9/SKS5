#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#undef MOUSE_MOVED
#include <GL\glut.h>
#include <time.h>
#include <vector>

using namespace std;

#define MAPZ 8
#define MAPXY 128
#define LEVELS 1

enum cubeColor { C_GREEN, C_RED, C_BLUE, C_ORANGE, C_BND, C_GRY, C_WHITE, C_NIL };
enum blockType { B_NIL, B_BND, B_WAL, B_KEY, B_TEL, B_COI, B_LGT, B_BUL, B_ZOM };
enum viewType  { V_NORTH, V_NORTHEAST, V_EAST, V_SOUTHEAST, V_SOUTH, V_SOUTHWEST, V_WEST, V_NORTHWEST };

typedef struct bulletType
{
	int x, y, z;
	viewType direction;
} bulletType;

#include "gfxengine.h"
#include "ioengine.h"
#include "minimap.h"
#include "zombie.h"
#include "scriptengine.h"
#include "inventory.h"

blockType uchartoblockType(unsigned char in);
unsigned int getscore();
bool canEat(int x, int y, int z, bool eatThatShit = false);
bool eatObject(int x, int y, int z);
void playerPlaceBlock(char pos = 1);
void makeMove(unsigned char type);
int mouseRotate();
unsigned char translateMap(char c);
char untranslateMap(unsigned char c);
bool loadMap(char *path);
bool loadMap(int lnum);
void randomMap();
void removeBullets(vector<int> removals);
void moveBullets();

/* Global variables */
extern const char *title, *title2;
extern GLfloat Yangle, Zangle;
extern bool jumping;
extern int mouseX, mouseY;
extern bool gameStarted, gameOver;
extern int requiredKeys;
extern float telr, telg, telb;
extern unsigned long frameCounter;
extern clock_t frameTime;
extern float frameRate;
extern vector<bulletType> bullets;
extern vector<zombieType *> zombies;
extern vector<char *> debugbuf;
extern bool debugVisible;
/*env start*/
extern viewType pl_view;
extern int pl_xpos, pl_ypos, pl_zpos;
extern int pl_keys, pl_coins, pl_walls, pl_lvl, pl_zkills, pl_health;
extern bool pl_hulk_smash;
extern unsigned char map[MAPZ][MAPXY][MAPXY];
extern char levelNames[LEVELS][128];
/*env end*/

#endif // ENGINE_H_INCLUDED
