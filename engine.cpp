#include "engine.h"
#include <stdio.h>
#include <time.h>

blockType uchartoblockType(unsigned char in)
{
	return (blockType(in));
}

unsigned int getscore()
{
    return (pl_walls * pl_coins) + (pl_keys * 5) + (pl_coins * pl_lvl) + (pl_zkills * pl_lvl);
}

bool canEat(int x, int y, int z, bool eatThatShit)
{
	blockType t = uchartoblockType(map[z][y][x]);
	if (t == B_NIL)
		return true;
	else if (t == B_KEY)
	{
		if (eatThatShit)
		{
			++pl_keys;
			map[z][y][x] = 0;
		}
		return true;
	}
	else if (t == B_COI)
	{
		if (eatThatShit)
		{
			++pl_coins;
			map[z][y][x] = 0;
		}
		return true;
	}
	else if (t == B_WAL && pl_coins >= 5)
	{
		if (eatThatShit)
		{
			pl_coins -= 5;
			++pl_walls;
			map[z][y][x] = 0;
			pl_hulk_smash = false;
		}
		return true;
	}
	else if (t == B_TEL)
	{
		if (pl_keys < requiredKeys)
			return false;
		++pl_lvl;
		pl_keys -= requiredKeys;
		pl_xpos = pl_ypos = 1;
		if (loadMap(pl_lvl) == false)
		{
			randomMap();
			//gameStarted = false;
			//gameOver = true;
		}
		return true;
	}
	else if (t == B_LGT)
	{
		return true;
	}
	return false;
}

bool eatObject(int x, int y, int z)
{
	if (canEat(x, y, z) && canEat(x, y, z + 1))
	{
		canEat(x, y, z, true);
		canEat(x, y, z + 1, true);
		return true;
	}
	return false;
}

void playerPlaceBlock(char pos) // 1 = right in front, 0 = z-1, 2 = z+1
{
	int x = pl_xpos, y = pl_ypos, z = pl_zpos;
	switch (pl_view)
	{
	case V_NORTH:
		y++;
		break;
	case V_SOUTH:
		y--;
		break;
	case V_EAST:
		x++;
		break;
	case V_WEST:
		x--;
		break;
	}
	if (pos == 2)
		z++;
	else if (pos == 0)
		z--;
	if (uchartoblockType(map[z][y][x]) == B_NIL)
	{
		if (pl_walls > 0)
		{
			--pl_walls;
			map[z][y][x] = 2; //<--------------------------------------write a function for me!
		}
	}
}

void makeMove(unsigned char type)
{
	//bit 0 : y- 0x01
	//bit 1 : y+ 0x02
	//bit 2 : x- 0x04
	//bit 3 : x+ 0x08
	//bit 4 : z- 0x10
	//bit 5 : z+ 0x20
	jumping = false;
	if (type & 0x01)
		pl_ypos--;
	if (type & 0x02)
		pl_ypos++;
	if (type & 0x04)
		pl_xpos--;
	if (type & 0x08)
		pl_xpos++;
	if (type & 0x10)
		pl_zpos--;
	if (type & 0x20)
		pl_zpos++;
}

int mouseRotate()
{
	int mx = mouseX / 16;
	return mx;
}

unsigned char translateMap(char c)
{
	switch (c)
	{
	case 'N':
		return 0;
	case 'B':
		return 1;
	case 'W':
		return 2;
	case 'K':
		return 3;
	case 'T':
		return 4;
	case 'C':
		return 5;
	case 'L':
		return 6;
	}
	return 0;
}

char untranslateMap(unsigned char c)
{
	switch (c)
	{
	case 0:
		return 'N';
	case 1:
		return 'B';
	case 2:
		return 'W';
	case 3:
		return 'K';
	case 4:
		return 'T';
	case 5:
		return 'C';
	case 6:
		return 'L';
	case 8:
		return 'Z';
	}
}

bool loadMap(char *path)
{
     //N B W K T C L
     char c;
     FILE *f = fopen(path, "r");
     if (f == NULL) return false;
     for (int z = 0; z < MAPZ; z++)
	{
		for (int y = 0; y < MAPXY; y++)
		{
			for (int x = 0; x < MAPXY; x++)
			{
				fscanf(f, "%c ", &c);
				map[z][y][x] = translateMap(c);
			}
		}
	}
	fscanf(f, "%d", &requiredKeys);
	fclose(f);
}

bool loadMap(int lnum)
{
	zombies.clear();
	return loadMap(levelNames[lnum]);
}

void randomMap()
{
	int keys = 0;
	zombies.clear();
	srand(time(NULL));
	for (int z = 1; z < MAPZ - 1; z++)
	{
		for (int y = 3; y < MAPXY - 1; y++)
		{
			for (int x = 3; x < MAPXY - 1; x++)
			{
				int num = rand() % 500;
				if (num == 1) num = 0; //no BND
				if (num == 4) num = 0; //no TEL
				if (num > 6 && num != 8) num = rand() % 25 == 0 ? 2 : 0;
				if (num == 8)
				{
					if (zombies.size() <= pl_lvl)
					{
						zombieType *zombie = new zombieType(x, y, z, rand() % 10 + 1, true);
						zombies.push_back(zombie);
					}
					else
						num = 0;
				}
				if (num == 3) ++keys;
				if (uchartoblockType(map[z - 1][y][x]) != B_ZOM)
					map[z][y][x] = (unsigned char)num;
			}
		}
	}
	int z, y, x;
	z = rand() % (MAPZ - 3) + 1;
	y = rand() % (MAPXY - 3) + 1;
	x = rand() % (MAPXY - 3) + 1;
	requiredKeys = keys - (keys / (pl_lvl == 0 ? 1 : pl_lvl));
	map[z][y][x] = 4; //place a teleport
	//map[1][3][3] = 8;
}

void removeBullets(vector<int> removals)
{
	vector<bulletType> newBullets;
	bool good;
	for (int i = 0; i < bullets.size(); i++)
	{
		good = true;
		for (int j = 0; j < removals.size(); j++)
		{
			if (removals[j] == i) good = false;
		}
		if (good)
			newBullets.push_back(bullets[i]);
	}
	bullets.clear();
	bullets = newBullets;
}

void moveBullets()
{
	vector<int> removals;
	for (int i = 0; i < bullets.size(); i++)
	{
		switch (bullets[i].direction)
		{
		case V_NORTH:
			++bullets[i].y;
			break;
		case V_SOUTH:
			--bullets[i].y;
			break;
		case V_EAST:
			++bullets[i].x;
			break;
		case V_WEST:
			--bullets[i].x;
			break;
		}
		if (uchartoblockType(map[bullets[i].z][bullets[i].y][bullets[i].x]) != B_NIL ||
			uchartoblockType(map[bullets[i].z - 1][bullets[i].y][bullets[i].x]) == B_ZOM)
		{
			if (zombieHere(bullets[i].x, bullets[i].y, bullets[i].z - 1))
				zombieHere(bullets[i].x, bullets[i].y, bullets[i].z - 1)->takeAttack();
			removals.push_back(i);
		}
	}
	removeBullets(removals);
}
