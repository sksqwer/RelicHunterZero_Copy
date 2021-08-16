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
			GRENADE = 47, HEAVYAMMO = 48, MEDIUMAMMO = 49, LIGHTAMMO = 50, HEAL = 51, BIGHEAL  = 52, SHIELD = 53};
	int kind = 41;
	POINT pos = { 0, 0 };
	int rotation = 0;
};

class item_ingame : public item_info
{
public:
	bool activate = true;
	POINT center = { 50, 50 };
	POINT size = { 100, 100 };
	//function
	item_ingame& operator=(const item_info& mapobj);
	void animation_set();
};
#endif // !_ITEM_INFO_H_
