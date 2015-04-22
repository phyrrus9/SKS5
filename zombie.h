#ifndef ZOMBIE_H_INCLUDED
#define ZOMBIE_H_INCLUDED

class zombieType
{
public:
	int x, y, z;
	int health;
	bool AI, dead;
	zombieType(int xp, int yp, int zp, int strength, bool AIp = false);
	void doAI(int xp, int yp, int zp);
	void doAttack();
	void takeAttack();
	bool canMove(int xp, int yp, int zp);
};

DWORD WINAPI ZombieThread(LPVOID lparam);
void cleanupZombies();
zombieType * zombieHere(int x, int y, int z);

#endif // ZOMBIE_H_INCLUDED
