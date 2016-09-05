/*
 * Super Key Seeker 5: The lost dimension
 * --Ethan Laur
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <time.h>
#include "engine.h"
#include "map.h"

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
    DWORD OutputThreadId, ColorChangerId, ZombieThreadId;
    if (!loadMap(0))
    	randomMap();
    CreateThread(
		  NULL,                   // default security attributes
		  0,                      // use default stack size
		  OutputThreadCurses,           // thread function name
		  NULL,                   // argument to thread function
		  0,                      // use default creation flags
		  &OutputThreadId);   // returns the thread identifier
    CreateThread(
		  NULL,                   // default security attributes
		  0,                      // use default stack size
		  ColorChanger,           // thread function name
		  NULL,                   // argument to thread function
		  0,                      // use default creation flags
		  &ColorChangerId);   // returns the thread identifier
	CreateThread(
		  NULL,                   // default security attributes
		  0,                      // use default stack size
		  ZombieThread,           // thread function name
		  NULL,                   // argument to thread function
		  0,                      // use default creation flags
		  &ZombieThreadId);   // returns the thread identifier
	srand(time(0));
	argc = 0;
	keyboard_enter(default_keyPressed);
	keyboard_enter(default_keySpecial, true);
	gfx_enter(default_display);
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(1024, 768);   // Set the window's initial width & height
    glutInitWindowPosition(525, 0); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutIdleFunc(display);
    HWND win=FindWindowA(NULL,title);//Find the window you created with GLUT
	unsigned int style=GetWindowLong(win,GWL_STYLE);//Get the current style
	style&=~WS_MAXIMIZEBOX;//Disable the maximize box
	style&=~WS_THICKFRAME;
	SetWindowLong(win,GWL_STYLE,style);//Set the new style
    initGL();                       // Our own OpenGL initialization
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(keySpecial);
    glutPassiveMotionFunc(mouseMove);
    glutWarpPointer(1024/2, 768/2);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
