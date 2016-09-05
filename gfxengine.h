#ifndef GFXENGINE_H_INCLUDED
#define GFXENGINE_H_INCLUDED

#define _WIN32_WINNT 0x1000
#include <GL\glut.h>
#include <windows.h>
#include "curses.h"

typedef void (*gfxRenderer)();
enum polygonType { P_Dodecahedron, P_Icosahedron, P_Octahedron,  P_Tetrahedron };

#define ROT_X 0x01
#define ROT_Y 0x02
#define ROT_Z 0x04

void gfx_enter(gfxRenderer);
void gfx_exit();
void initGL();
void rotateScene();
void setColor(cubeColor c);
void drawCube(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate = 0);
void drawSphere(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate = 0);
void drawTourus(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate = 0);
void drawTeapot(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate = 0);
void drawPolygon(polygonType t, float xpos, float ypos, float zpos, float size, cubeColor color, int rotate = 0);
void placeObject(blockType t, int distx, int disty, int distz = 0);
void default_display();
void display();
void reshape(GLsizei width, GLsizei height);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void refresh_win(WINDOW *win);
DWORD WINAPI ColorChanger(LPVOID lparam);
void displayText( float x, float y, int r, int g, int b, const char *str);

#endif // GFXENGINE_H_INCLUDED
