#include "bullet.h"

void Bullet::set()
{
	bullet_effect_row = 0;
	state = START;
	switch (kind)
	{
		//LIGHT, NORMAL, HEAVY, SNIPER, ENEMY };
	case 0: // LIGHT
		row = 1;
		bullet_effect_col = 43;
		break;
	case 1: // NORMAL
		row = 2;
		bullet_effect_col = 44;
		break;
	case 2: // HEAVY
		row = 0;
		bullet_effect_col = 45;
		break;
	case 3: // SNIPER
		row = 3;
		bullet_effect_col = 45;
		break;
	case 4: // ENEMY
		row = 4;
		bullet_effect_col = 45;
		break;
	}
}
