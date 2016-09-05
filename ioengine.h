#ifndef IOENGINE_H_INCLUDED
#define IOENGINE_H_INCLUDED

#define _WIN32_WINNT 0x1000
#include <windows.h>
#include "curses.h"

typedef void (*keyEvent)(int, int, int);

void keyboard_enter(keyEvent k, bool special = false);
void keyboard_exit(bool special = false);
void clearConsole();
void print_messages(WINDOW *out);
void print_status(WINDOW *out);
void print_debug(WINDOW *out);
DWORD WINAPI OutputThreadCurses(LPVOID lparam);
void mouseMove(int x, int y);
void keyPressed (unsigned char key, int x, int y);
void default_keyPressed (int key, int x, int y);
void default_keySpecial(int key, int x, int y);
void keySpecial(int key, int x, int y);
void clean_debug_buffer();
void writeDebug(const char *fmt, ...);

#endif // IOENGINE_H_INCLUDED
