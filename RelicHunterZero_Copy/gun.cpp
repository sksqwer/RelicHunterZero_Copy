#include "gun.h"

void Gun::set_gun()
{
	// PISTOL, SMG, SHOTGUN, RIFLE, SNIPER, MACHINEGUN 
	switch (kind)
	{
	case 0: // PISTOL
		impact = 10;
		range = 800;
		rate = 100;
		maxbullet = 30;
		curbullet = 30;
		bullet_cost = 1;
		bullet_type = 1;
		bullet_speed = 50;
		break;
	case 1: // SMG
		break;
	case 2: // SHOTGUN
		break;
	case 3: // RIFLE
		break;
	case 4: // SNIPER
		break;
	case 5: // MACHINEGUN
		break;
	}
}
