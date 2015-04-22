#include "engine.h"

void display_minimap_NORTH(WINDOW *win, unsigned char **minimap)
{
	int curx, cury;
	for (int y = 20; y >= 0; y--)
	{
		for (int x = 0; x < 21; x++)
		{
			if (minimap[y][x] == 0xff)
			{
				start_color();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				wattron(win, COLOR_PAIR(1));
				refresh();
				wprintw(win, "@ ");
				wattroff(win, COLOR_PAIR(1));
			}
			else if (uchartoblockType(minimap[y][x]) != B_NIL) //draw it
				wprintw(win, "%c ", untranslateMap(minimap[y][x]));
			else
			{
				getyx(win, cury, curx);
				wmove(win, cury, ++curx);
				wprintw(win, " ");
			}
		}
		wprintw(win, "\n ");
	}
}

void display_minimap_SOUTH(WINDOW *win, unsigned char **minimap)
{
	int curx, cury;
	for (int y = 0; y < 21; y++)
	{
		for (int x = 20; x >= 0; x--)
		{
			if (minimap[y][x] == 0xff)
			{
				start_color();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				wattron(win, COLOR_PAIR(1));
				wprintw(win, "@ ");
				wattroff(win, COLOR_PAIR(1));
			}
			else if (uchartoblockType(minimap[y][x]) != B_NIL) //draw it
				wprintw(win, "%c ", untranslateMap(minimap[y][x]));
			else
			{
				getyx(win, cury, curx);
				wmove(win, cury, ++curx);
				wprintw(win, " ");
			}
		}
		wprintw(win, "\n ");
	}
}

void display_minimap_EAST(WINDOW *win, unsigned char **minimap)
{
	int curx, cury;
	for (int y = 20; y >= 0; y--)
	{
		for (int x = 20; x >= 0; x--)
		{
			if (minimap[x][y] == 0xff)
			{
				start_color();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				wattron(win, COLOR_PAIR(1));
				wprintw(win, "@ ");
				wattroff(win, COLOR_PAIR(1));
			}
			else if (uchartoblockType(minimap[x][y]) != B_NIL) //draw it
				wprintw(win, "%c ", untranslateMap(minimap[x][y]));
			else
			{
				getyx(win, cury, curx);
				wmove(win, cury, ++curx);
				wprintw(win, " ");
			}
		}
		wprintw(win, "\n ");
	}
}

void display_minimap_WEST(WINDOW *win, unsigned char **minimap)
{
	int curx, cury;
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x < 21; x++)
		{
			if (minimap[x][y] == 0xff)
			{
				start_color();
				init_pair(1, COLOR_RED, COLOR_BLACK);
				wattron(win, COLOR_PAIR(1));
				wprintw(win, "@ ");
				wattroff(win, COLOR_PAIR(1));
			}
			else if (uchartoblockType(minimap[x][y]) != B_NIL) //draw it
				wprintw(win, "%c ", untranslateMap(minimap[x][y]));
			else
			{
				getyx(win, cury, curx);
				wmove(win, cury, ++curx);
				wprintw(win, " ");
			}
		}
		wprintw(win, "\n ");
	}
}

void draw_minimap(WINDOW *win)
{
	int ystart = pl_ypos - 10, yend = pl_ypos + 10;
	int xstart = pl_xpos - 10, xend = pl_xpos + 10;
	unsigned char **minimap = new unsigned char *[23];
	for (int i = 0; i < 23; i++)
		minimap[i] = new unsigned char[23];
	for (int z = 1; z < MAPZ - 1; z++)
		for (int y = ystart, yp = 0; y <= yend; y++, yp++)
			for (int x = xstart, xp = 0; x <= xend; x++, xp++)
				minimap[yp][xp] = B_NIL;
	for (int z = 1; z < MAPZ - 1; z++)
		for (int y = ystart, yp = 0; y <= yend; y++, yp++)
			for (int x = xstart, xp = 0; x <= xend; x++, xp++)
			{
				if ((y < 0 || y > MAPXY) || (x < 0 || x > MAPXY))
					minimap[yp][xp] = 0; //B_NIL
				else if (y == pl_ypos && x == pl_xpos)
					minimap[yp][xp] = 0xff;
				else if (uchartoblockType(map[z][y][x]) != B_NIL)
					minimap[yp][xp] = map[z][y][x];
			}
	wclear(win);
	wprintw(win, "\n ");
	if (pl_view == V_NORTH)
		display_minimap_NORTH(win, minimap);
	else if (pl_view == V_SOUTH)
		display_minimap_SOUTH(win, minimap);
	else if (pl_view == V_EAST)
		display_minimap_EAST(win, minimap);
	else if (pl_view == V_WEST)
		display_minimap_WEST(win, minimap);
	refresh_win(win);
	for (int i = 0; i < 23; i++)
		delete minimap[i];
	delete minimap;
	refresh();
}
