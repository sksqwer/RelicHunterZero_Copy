#include "bullet.h"

void Bullet::set()
{
	switch (kind)
	{
		//LIGHT, NORMAL, HEAVY, SNIPER, ENEMY };
	case 0: // LIGHT
		row = 1;
		break;
	case 1: // NORMAL
		row = 2;
		break;
	case 2: // HEAVY
		row = 0;
		break;
	case 3: // SNIPER
		row = 3;
		break;
	case 4: // ENEMY
		row = 4;
		break;
	}
}
