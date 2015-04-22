#include "engine.h"

#include <io.h>
#include <fcntl.h>
#include <wincon.h>
#include <stdio.h>
#include <string>
#include <stack>
#include <stdarg.h>
#include "curses.h"

using namespace std;

string scriptStr = ""; //yes, I know it's file global
bool scriptReading = false;

void clearConsole()
{
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
	   console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
	   console, FOREGROUND_GREEN/* | FOREGROUND_RED | FOREGROUND_BLUE*/,
	   screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void print_messages(WINDOW *out)
{
	wclear(out);
	if (!gameOver)
	{
		wprintw(out, "\n"
				" Welcome to Super Key Seeker 5: The Lost Dimension\n"
				" This game is very simple, let's walk through the controls\n"
				" w - Move forward     a - Move left     s - Move backward\n"
				" d - Move right   SPACE - Jump\n"
				" Arrow keys:\n"
				" LT - Look left       RT - Look right   UP - Look up\n"
				" Mouse: Look around\n");
		if (!gameStarted)
			wprintw(out, " Press END to quit the game, or S to start now!\n");
	}
	else
		wprintw(out, "\n"
				   " Well, it seems this is the end of the line\n"
				   " Press the END key to exit ");
	refresh_win(out);
}

void print_status(WINDOW *out)
{
	wclear(out);
	wprintw(out, "\n");
	wprintw(out, " K: %d\tC: %d\tW: %d\tS: %d\tLVL: %d\n", pl_keys, pl_coins, pl_walls, getscore(), pl_lvl);
	wprintw(out, " HULKMODE: %d\tZROT: %-2d\tYROT: %-2d\n", pl_hulk_smash, -(int)Zangle, abs((int)Yangle));
	wprintw(out, " KREQ: %-3d\tFPS: %2.2f\tKILL: %d\n", requiredKeys, frameRate, pl_zkills);
	wprintw(out, " HEALTH: %d %s\n", pl_health, scriptStr.c_str());
	refresh_win(out);
}

void clean_debug_buffer()
{
	int start = debugbuf.size() - 20;
	if (start < 0) start = 0;
	debugbuf.erase(debugbuf.begin(), debugbuf.begin() + start);
}

void print_debug(WINDOW *out)
{
	if (!debugVisible) return;
	wclear(out);
	wprintw(out, "\n");
	wprintw(out, " X: %d\tY: %d\tZ: %d\tMx: %-3d\tMy: %-3d\n", pl_xpos, pl_ypos, pl_zpos, mouseX, mouseY);
	if (debugbuf.size() > 80)
		clean_debug_buffer();
	int start = debugbuf.size() - 20;
	if (start < 0) start = 0;
	for (int i = start; i < debugbuf.size(); i++)
		wprintw(out, " %s\n", debugbuf[i]);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(out, COLOR_PAIR(1));
	refresh_win(out);
	wattroff(out, COLOR_PAIR(1));
}

DWORD WINAPI OutputThreadCurses(LPVOID lparam)
{
	char *title2 = new char[strlen(title) + strlen(" - Console") + 5];
	bool debugMode = debugVisible;
	SCREEN *scr;
	sprintf(title2, "%s - Console", title);
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	HWND ConsoleHandle = GetConsoleWindow();
	MoveWindow(ConsoleHandle, 0, 0, 525, 535, true);
	SetConsoleTitleA(title2);
	//*stdin = *hf_in;
	//*stdout = *hf_out;
	HWND windowHandle = GetForegroundWindow();
	long Style = GetWindowLong(windowHandle, GWL_STYLE);
	Style &= ~WS_MAXIMIZEBOX; //this makes it still work when WS_MAXIMIZEBOX is actually already toggled off
	Style&=~WS_THICKFRAME;
	SetWindowLong(windowHandle, GWL_STYLE, Style);
	FILE *out = fopen("stdout.txt", "w");
	*stdout = *out;
	*stderr = *out;
	//initscr();
	scr = newterm("VT100", hf_in, hf_out);
	set_term(scr);
	refresh();
	WINDOW *minimap = create_newwin(23, 44, 0, 0);
	WINDOW *status  = create_newwin(6, 46, 23, 0);
	WINDOW *message = create_newwin(10, 60, 29, 0);
	WINDOW *debugwin = create_newwin(23, 60, 39, 0);
	for (;;)
	{
		if (debugMode != debugVisible)
		{
			endwin();
			debugMode = debugVisible;
			if (debugMode)
				MoveWindow(ConsoleHandle, 0, 0, 525, 810/*550*/, true);
			else
				MoveWindow(ConsoleHandle, 0, 0, 525, 535, true);
			scr = newterm("VT100", hf_in, hf_out);
			set_term(scr);
			refresh();
			minimap = create_newwin(23, 44, 0, 0);
			status  = create_newwin(6, 46, 23, 0);
			message = create_newwin(10, 60, 29, 0);
			debugwin = create_newwin(23, 60, 39, 0);
		}
		print_messages(message);
		print_status(status);
		draw_minimap(minimap);
		print_debug(debugwin);
		Sleep(100);
	}
	endwin();
}

void mouseMove(int x, int y)
{
	if (!gameStarted) return;
	mouseX = x - (1024 / 2);
	mouseY = y - (768 / 2);
	Zangle = mouseY / 8;
	//Yangle = mouseRotate();
	glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y)
{
	if (gameOver) return;
	if (!gameStarted)
	{
		if (key == 'S')
		{
			gameStarted = true;
			glutWarpPointer(1024/2, 768/2);
		}
          return; //parse keys until the game starts
	}
	if (key == '/' && !scriptReading)
		scriptReading = true;
	else if (scriptReading)
	{
		if (key != '\r' && key != '\n')
			scriptStr += key;
		else
		{
			scriptReading = false;
			parseScriptLine((char *)scriptStr.c_str());
			scriptStr = "";
		}
		return;
	}
	glutWarpPointer(1024/2, (768/2) + mouseY);
	glutSetCursor(GLUT_CURSOR_NONE);
	if (key == ' ')
	{
		if (canEat(pl_xpos, pl_ypos, pl_zpos + 1) && !jumping)
		{
			makeMove(32);
			glutPostRedisplay();
			jumping = true;
			return;
		}
	}
	if (key == 'f') //fire gun
	{
		bulletType bullet;
		bullet.direction = pl_view;
		bullet.x = pl_xpos;
		bullet.y = pl_ypos;
		bullet.z = pl_zpos + 1;
		bullets.push_back(bullet);
	}
	else if (key == 'l') //lay block
	{
		playerPlaceBlock(1);
		glutPostRedisplay();
		return;
	}
	else if (key == 'p') //place block
	{
		playerPlaceBlock(2);
		glutPostRedisplay();
		return;
	}
	else if (key == ',') //place block
	{
		playerPlaceBlock(0);
		glutPostRedisplay();
		return;
	}
	/*else if (key == 'q')
	{
		if (pl_view == V_NORTH && eatObject(pl_xpos - 1, pl_ypos + 1, pl_zpos))
			makeMove(2 | 4);
		else if (pl_view == V_SOUTH && eatObject(pl_xpos + 1, pl_ypos - 1, pl_zpos))
			makeMove(1 | 8);
		else if (pl_view == V_EAST && eatObject(pl_xpos + 1, pl_ypos + 1, pl_zpos))
			makeMove(2 | 8);
		else if (pl_view == V_WEST && eatObject(pl_xpos - 1, pl_ypos - 1, pl_zpos))
			makeMove(1 | 4);
	}
	else if (key == 'e')
	{
		if (pl_view == V_NORTH && eatObject(pl_xpos - 1, pl_ypos, pl_zpos) && eatObject(pl_xpos + 1, pl_ypos, pl_zpos))
			makeMove(2 | 8);
	}*/ //in progress
	else if (key == 'w')
	{
		if (pl_view == V_NORTH && eatObject(pl_xpos, pl_ypos + 1, pl_zpos))
			makeMove(2);
		else if (pl_view == V_SOUTH && eatObject(pl_xpos, pl_ypos - 1, pl_zpos))
			makeMove(1);
		else if (pl_view == V_EAST && eatObject(pl_xpos + 1, pl_ypos, pl_zpos))
			makeMove(8);
		else if (pl_view == V_WEST && eatObject(pl_xpos - 1, pl_ypos, pl_zpos))
			makeMove(4);
    	}
    	else if (key == 'a')
    	{
		if (pl_view == V_NORTH && eatObject(pl_xpos - 1, pl_ypos, pl_zpos))
			makeMove(4);
		else if (pl_view == V_SOUTH && eatObject(pl_xpos + 1, pl_ypos, pl_zpos))
			makeMove(8);
		else if (pl_view == V_EAST && eatObject(pl_xpos, pl_ypos + 1, pl_zpos))
			makeMove(2);
		else if (pl_view == V_WEST && eatObject(pl_xpos, pl_ypos - 1, pl_zpos))
			makeMove(1);
	}
    	else if (key == 's')
    	{
		if (pl_view == V_NORTH && eatObject(pl_xpos, pl_ypos - 1, pl_zpos))
			makeMove(1);
		else if (pl_view == V_SOUTH && eatObject(pl_xpos, pl_ypos + 1, pl_zpos))
			makeMove(2);
		else if (pl_view == V_EAST && eatObject(pl_xpos - 1, pl_ypos, pl_zpos))
			makeMove(4);
		else if (pl_view == V_WEST && eatObject(pl_xpos + 1, pl_ypos, pl_zpos))
			makeMove(8);
    	}
    	else if (key == 'd')
    	{
		if (pl_view == V_NORTH && eatObject(pl_xpos + 1, pl_ypos, pl_zpos))
			makeMove(8);
		else if (pl_view == V_SOUTH && eatObject(pl_xpos - 1, pl_ypos, pl_zpos))
			makeMove(4);
		else if (pl_view == V_EAST && eatObject(pl_xpos, pl_ypos - 1, pl_zpos))
			makeMove(1);
		else if (pl_view == V_WEST && eatObject(pl_xpos, pl_ypos + 1, pl_zpos))
			makeMove(2);
    	}
    	if (pl_zpos > 1)
    	{
		while (canEat(pl_xpos, pl_ypos, pl_zpos - 1, false) && uchartoblockType(map[pl_zpos - 1][pl_ypos][pl_xpos]) != B_WAL)
		{
			eatObject(pl_xpos, pl_ypos, pl_zpos);
			makeMove(16);
		}
	}
	glutPostRedisplay();
}

void keySpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_INSERT)
		debugVisible = debugVisible ? false : true;
	if (!gameStarted || gameOver)
	{
		if (key == GLUT_KEY_END)
			exit(0);
		if (key == GLUT_KEY_HOME)
			gameStarted = true;
		return;
	}
	else if (key == GLUT_KEY_HOME)
	{
		gameStarted = false;
		return;
	}
    switch (key)
    {
    case GLUT_KEY_UP:
	   Zangle --;
	   break;
    case GLUT_KEY_LEFT:
	   Yangle -= 90;
	   break;
    case GLUT_KEY_RIGHT:
	   Yangle += 90;
	   break;
    case GLUT_KEY_DOWN:
	   Zangle ++;
	   break;
    case GLUT_KEY_END:
	   exit(0);
	   break;
    }
    while (Yangle >= 360) Yangle -= 360;
    //while (Yangle >= -360) Yangle += 360;
    while (Yangle < 0) Yangle += 360;
    if (Yangle == 0)
	   pl_view = V_NORTH;
    if (Yangle == 90 || Yangle == -270)
	   pl_view = V_EAST;
    if (Yangle == 180 || Yangle == -180)
	   pl_view = V_SOUTH;
    if (Yangle == 270 || Yangle == -90)
	   pl_view = V_WEST;
    glutPostRedisplay();
}

void writeDebug(const char *fmt, ...)
{
	char *buf = new char[60];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf,fmt, args);
	perror (buf);
	va_end (args);
	debugbuf.push_back(buf);
}
