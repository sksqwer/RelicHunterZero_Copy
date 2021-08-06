#pragma once
#ifndef _ITEM_INFO_H_
#define _ITEM_INFO_H_

#include "framework.h"
#include "MapObject_info.h"

class item_info : public MapObject_info
{
public:
//	enum { GUN = 41, AMMO = 42, RECOVERY = 43 };
	enum { PISTOL = 41, SMG = 42, SHOTGUN = 43, RIFLE = 44, SNIPER = 45, MACHINEGUN = 46,
			GREMADE = 47, HEAVYAMMO = 48, MEDIUMAMMO = 49, LIGHTAMMO = 50, HEAL = 51, BIGHEAL  = 52, SHIELD = 53};
	int kind = 41;
	POINT pos = { 0, 0 };
	int rotation = 0;
};

#endif // !_ITEM_INFO_H_
