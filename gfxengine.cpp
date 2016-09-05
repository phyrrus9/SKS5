#include "engine.h"
#include <math.h>
#include <limits.h>
#include <stack>

using std::stack;

int angle = 0;
stack<gfxRenderer> gfx_renderer;

void gfx_enter(gfxRenderer r)
{
	gfx_renderer.push(r);
}
void gfx_exit()
{
	gfx_renderer.pop();
}

void initGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);  // Nice perspective corrections
}

void rotateScene()
{
	glTranslatef(0, 0, 0);
	glRotatef(Yangle + mouseRotate(), 0, 1, 0);
	if (pl_view == V_NORTH)
		glRotatef(Zangle, 1, 0, 0);
	else if (pl_view == V_SOUTH)
		glRotatef(-Zangle, 1, 0, 0);
	else if (pl_view == V_WEST)
		glRotatef(-Zangle, 0, 0, 1);
	else
		glRotatef(Zangle, 0, 0, 1);
    glRotatef(mouseRotate(), 0, 1, 0);
}

void setColor(cubeColor c)
{
	switch (c)
	{
	case C_GREEN:
		glColor3f(0.0f, 1.0f, 0.0f);
		break;
    	case C_RED:
	   	glColor3f(1.0f, 0.0f, 0.0f);
	   	break;
    	case C_BLUE:
	   	glColor3f(0.0f, 0.0f, 1.0f);
	   	break;
    	case C_ORANGE:
	   	glColor3f(1.0f, 0.5f, 0.0f);
	   	break;
	case C_GRY:
		glColor3f(0.1, 0.1, 0.1);
		break;
    	case C_BND:
	   	glColor3f(0.001f, 0.001f, 0.001f);
	   	break;
	case C_WHITE:
		glColor3f(1.0f, 1.0f, 1.0f);
		break;
	case C_NIL:
		break;
    	}
}

void drawCube(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate)
{
    glLoadIdentity();                 // Reset the model-view matrix
    rotateScene();
    glTranslatef(xpos, ypos, zpos);  // Move right and into the screen
    if (rotate) glRotatef(angle, rotate & ROT_X ? 1 : 0, rotate & ROT_Y ? 1 : 0, rotate & ROT_Z ? 1 : 0);
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    setColor(color);
    glVertex3f( size, size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size,  size);
    glVertex3f( size, size,  size);
    glVertex3f( size, -size,  size);
    glVertex3f(-size, -size,  size);
    glVertex3f(-size, -size, -size);
    glVertex3f( size, -size, -size);
    glVertex3f( size,  size, size);
    glVertex3f(-size,  size, size);
    glVertex3f(-size, -size, size);
    glVertex3f( size, -size, size);
    glVertex3f( size, -size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size,  size, -size);
    glVertex3f( size,  size, -size);
    glVertex3f(-size,  size,  size);
    glVertex3f(-size,  size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size,  size);
    glVertex3f(size,  size, -size);
    glVertex3f(size,  size,  size);
    glVertex3f(size, -size,  size);
    glVertex3f(size, -size, -size);
   glEnd();  // End of drawing color-cube
}

void drawSphere(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate)
{
    glLoadIdentity();                 // Reset the model-view matrix
    rotateScene();
    glTranslatef(xpos, ypos, zpos);  // Move right and into the screen
    if (rotate) glRotatef(angle, rotate & ROT_X ? 1 : 0, rotate & ROT_Y ? 1 : 0, rotate & ROT_Z ? 1 : 0);
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    setColor(color);
    glutSolidSphere(size, 20, 20);
   glEnd();  // End of drawing color-cube
}

void drawTourus(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate)
{
    glLoadIdentity();                 // Reset the model-view matrix
	rotateScene();
    glTranslatef(xpos, ypos, zpos);  // Move right and into the
    if (rotate) glRotatef(angle, rotate & ROT_X ? 1 : 0, rotate & ROT_Y ? 1 : 0, rotate & ROT_Z ? 1 : 0);
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    setColor(color);
    glutSolidTorus(size/16, size, 10, 50);
   glEnd();  // End of drawing color-cube
}

void drawTeapot(float xpos, float ypos, float zpos, float size, cubeColor color, int rotate)
{
    glLoadIdentity();                 // Reset the model-view matrix
    rotateScene();
    glTranslatef(xpos, ypos, zpos);  // Move right and into the screen
    if (rotate) glRotatef(angle, rotate & ROT_X ? 1 : 0, rotate & ROT_Y ? 1 : 0, rotate & ROT_Z ? 1 : 0);
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    setColor(color);
    glutSolidTeapot(size);
   glEnd();  // End of drawing color-cube
}

void drawPolygon(polygonType t, float xpos, float ypos, float zpos, float size, cubeColor color, int rotate)
{
	glLoadIdentity();
	rotateScene();
	glTranslatef(xpos, ypos, zpos);
	if (rotate) glRotatef(angle, rotate & ROT_X ? 1 : 0, rotate & ROT_Y ? 1 : 0, rotate & ROT_Z ? 1 : 0);
	glScalef(size, size, size);
	glBegin(GL_POLYGON);
	setColor(color);
	switch (t)
	{
	case P_Dodecahedron:
		glutSolidDodecahedron();
		break;
	case P_Icosahedron:
		glutSolidIcosahedron();
		break;
	case P_Octahedron:
		glutSolidOctahedron();
		break;
	case P_Tetrahedron:
		glutSolidTetrahedron();
		break;
	}
	glEnd();
	glScalef(1, 1, 1);
}

void placeObject(blockType t, int distx, int disty, int distz)
{
	//double tanparam = (double)distx/(double)disty;
	//double angle = tan(tanparam);
	double distance = sqrt((double)(distx * distx) + (double(disty * disty)));
	if (distance > 25) return; //FOV don't draw
	switch (t)
	{
	case B_WAL:
		drawCube(distx, distz - 1, -(disty), .3, C_GRY);
		break;
	case B_KEY:
		drawPolygon(P_Icosahedron, distx, distz - 1, -(disty), .35, C_GREEN, ROT_X | ROT_Y);
		break;
	case B_COI:
		drawPolygon(P_Octahedron, distx, distz - 1, -(disty), .25, C_ORANGE);
		break;
	case B_TEL:
		drawTourus(distx, distz - 1, -(disty), .45, C_BLUE, ROT_Z);
		drawTourus(distx, distz - 1, -(disty), .45, C_BLUE, ROT_X);
		drawTourus(distx, distz - 1, -(disty), .45, C_BLUE, ROT_Y);
		glColor3f(telb, telr, telg);
		drawTourus(distx, distz - 1, -(disty), .45, C_NIL, ROT_Z | ROT_X);
		drawTourus(distx, distz - 1, -(disty), .45, C_NIL, ROT_X | ROT_Y);
		drawTourus(distx, distz - 1, -(disty), .45, C_NIL, ROT_Z | ROT_Y);
		glColor3f(telr, telg, telb);
		drawSphere(distx, distz - 1, -(disty), .3, C_NIL);
		break;
	case B_BND:
		drawCube(distx, distz - 1, -(disty), .45, C_BND);
		break;
	case B_LGT:
		drawPolygon(P_Tetrahedron, distx, distz - 1, -(disty), .3, C_WHITE);
		break;
	case B_BUL:
		drawSphere(distx, distz - 1, -(disty), .1, C_RED);
		break;
	case B_ZOM:
		drawCube(distx, distz - 1, -(disty), .35, C_RED);
		drawSphere(distx, distz, -(disty), .4, C_RED);
	case B_NIL:
		break;
    }
}

void default_display()
{
	if (pl_health <= 0)
		gameOver = true;
	++angle;
	++frameCounter;
	clock_t curr = clock();
	if ((curr - frameTime) / CLOCKS_PER_SEC > 1)
	{
		frameRate = (double)frameCounter / (double)((double)(curr - frameTime) / (double)CLOCKS_PER_SEC);
		if (frameCounter > LONG_MAX)
		{
			frameCounter = 0;
			frameTime = clock();
		}
	}
	moveBullets();
	int maxdist = 60;
	int 	zp = pl_zpos - maxdist,
		xp = pl_xpos - maxdist,
		yp = pl_ypos - maxdist;
	zp = zp < 0 ? 0 : zp;
	zp = zp < 0 ? 0 : xp;
	yp = yp < 0 ? 0 : yp;
	for (zp = 0; zp < pl_zpos + maxdist && zp < MAPZ; zp++)
	   for (xp = 0; xp < pl_xpos + maxdist && xp < MAPXY; xp++)
		  for (yp = 0; yp < pl_ypos + maxdist && yp < MAPXY; yp++)
				placeObject(uchartoblockType(map[zp][yp][xp]), (xp - pl_xpos), (yp - pl_ypos), (zp - pl_zpos));

	//drawCube(0, 0, -1, .001, C_GREEN); //crosshair
	//drawCube(0, 0, 1, .001, C_GREEN); //crosshair
	//drawCube(0, -1, -1, .001, C_BLUE); //crosshair
	for (unsigned int bul = 0; bul < bullets.size(); bul++)
		placeObject(B_BUL, (bullets[bul].x - pl_xpos) + .5, (bullets[bul].y - pl_ypos) + .5, (bullets[bul].z - pl_zpos));
}

void display()
{
	gfxRenderer r = gfx_renderer.top();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity();
	r();
	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height)
{  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void refresh_win(WINDOW *win)
{
	box(win, 0, 0);
	wrefresh(win);
}

DWORD WINAPI ColorChanger(LPVOID lparam)
{
	for (;;)
	{
		//teleport color change
		for (; telb > 0; telb -= .0005, telr += .0005) Sleep(1); //blue->red
		for (; telr > 0; telr -= .0005, telg += .0005) Sleep(1); //red->green
		for (; telg > 0; telg -= .0005, telb += .0005) Sleep(1); //green->blue
	}
}

void displayText( float x, float y, int r, int g, int b, const char *str )
{
	int j = strlen( str );
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, str[i] );
	}
}
