#include "engine.h"
#include <math.h>
#include <windows.h>
#include <stack>

using namespace std;

zombieType::zombieType(int xp, int yp, int zp, int strength, bool AIp)
{
	x = xp;
	y = yp;
	z = zp;
	health = strength;
	AI = AIp;
	dead = false;
}

bool zombieType::canMove(int xp, int yp, int zp)
{
	return
		(
			uchartoblockType(map[zp][yp][xp]) == B_NIL &&
			uchartoblockType(map[zp+1][yp][xp]) == B_NIL
		);
}

#define SQ(a) (a * a)

void zombieType::doAI(int xp, int yp, int zp)
{
	double	distx = abs(x - xp),
			disty = abs(y - yp),
			distz = abs(z - zp);
	double distance = sqrt((double)(SQ(distx)) + (double)(SQ(disty)));
	int nx = x, ny = y, nz = z;
	if (distance < 15 && distance > 2) //field of sight
	{
		Sleep(10);
		if (x - xp < 0 && canMove(x + 1, y, z))
			nx++;
		else if (x - xp > 0 && canMove(x - 1, y, z))
			nx--;
		if (y - yp < 0 && canMove(x, y + 1, z))
			ny++;
		else if (y - yp > 0 && canMove(x, y - 1, z))
			ny--;
		map[z][y][x] = 0;
		map[nz][ny][nx] = 8;
		z = nz;
		y = ny;
		x = nx;
		writeDebug("Z:(%x) at: %d %d %d -> %d %d %d = %d", this, x, y, z, (int)distx, (int)disty, (int)distz, (int)distance);
	}
	if (distance < 3)
		doAttack();
	while (uchartoblockType(map[z - 1][y][x]) == B_NIL) //gravity
	{
		map[z][y][x] = 0;
		--z;
		map[z][y][x] = 8;
	}
}

void zombieType::doAttack()
{
	writeDebug("Z:(%x) attacking player", this);
	pl_health--;
}

void zombieType::takeAttack()
{
	health--;
	writeDebug("Z:(%x) taking attack! (%d)", this, health);
	if (health <= 0)
	{
		writeDebug("Z:(%x) DEAD!", this);
		dead = true;
		++pl_zkills;
		map[z][x][y] = 0;
	}
}

void cleanupZombies()
{
	zombieType *tmp;
	for (int z = 0; z < MAPZ; z++)
	{
		for (int y = 0; y < MAPXY; y++)
		{
			for (int x = 0; x < MAPXY; x++)
			{
				if ((tmp = zombieHere(x, y, z)) == NULL && uchartoblockType(map[z][y][x]) == B_ZOM)
				{
					writeDebug("Cleaning up at %d %d %d", x, y, z);
					map[z][y][x] = 0;
				}
			}
		}
	}
	for (int i = 0; i < zombies.size(); i++)
	{
		if (zombies[i]->dead)
		{
			writeDebug("Removed dead body at (%x)", zombies[i]);
			delete zombies[i];
			zombies.erase(zombies.begin() + i);
			i = 0;
		}
	}
}

DWORD WINAPI ZombieThread(LPVOID lparam)
{
	stack<zombieType> tmp;
	int ctr = 0;
	for (;;)
	{
		while (gameStarted == false || gameOver == true) Sleep(500); //dont do anything until we begin
		++ctr;
		if (ctr >= 15)
		{
			ctr = 0;
			for (int i = 0; i < zombies.size(); i++)
			{
				if (!zombies[i]->dead)
					zombies[i]->doAI(pl_xpos, pl_ypos, pl_zpos);
			}
		}
		cleanupZombies();
		Sleep(100);
	}
}

zombieType * zombieHere(int x, int y, int z)
{
	for (int i = 0; i < zombies.size(); i++)
		if (zombies[i]->x == x)
			if (zombies[i]->y == y)
				if (zombies[i]->z == z)
					if (zombies[i]->dead == false)
						return zombies[i];
	return NULL;
}
