#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "curses.h"

void display_minimap_NORTH(WINDOW *win, unsigned char **minimap);
void display_minimap_SOUTH(WINDOW *win, unsigned char **minimap);
void display_minimap_EAST(WINDOW *win, unsigned char **minimap);
void display_minimap_WEST(WINDOW *win, unsigned char **minimap);
void draw_minimap(WINDOW *win);


#endif // MINIMAP_H_INCLUDED
