#include "gun.h"
#include "casing.h"

void Gun::set_gun()
{
	// PISTOL, SMG, SHOTGUN, RIFLE, SNIPER, MACHINEGUN 
	switch (kind)
	{
	case 0: // PISTOL
		impact = 10;
		range = 500;
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

int Gun::ret_bullet_type()
{
	switch (bullet_type)
	{
	case(0):
		return Casing::LIGHT;
		break;
	case(2):
		return Casing::MEDIUM;
		break;
	case(3):
	case(4):
		return Casing::HEAVY;
		break;
	}

	return Casing::LIGHT;
}
