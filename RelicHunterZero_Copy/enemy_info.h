#pragma once
#ifndef _ENEMY_INFO_H_
#define _ENEMY_INFO_H_

#include "framework.h"
#include "MapObject_info.h"

class enemy_info : public MapObject_info
{
public:
	enum { DUCK = 61, DUCK2= 62, DUCK3 = 63, KAMIKAZE1 = 64, KAMIKAZE2 = 65,
			TURTLE1 = 66, TURTLE2 = 67, TURTLE3 = 68, BOSS = 69};
	int kind = 61;
	POINT pos = { 0, 0 };
	int rotation = 0;
	POINT block = { 1, 1 };
	int framelimit = 0;
};

class enemy_ingame : public enemy_info
{
public:
	POINT center = { 50, 50 };
	POINT size = { 100, 100 };
	int HP = 100;

	//function
	enemy_ingame& operator=(const enemy_info& mapobj);
};

#endif // !_ENEMY_INFO_H_
