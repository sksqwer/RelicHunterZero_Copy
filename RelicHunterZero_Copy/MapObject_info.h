#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"

class MapObject_info
{
public:
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12};			// attr
	POINT pos = { 0 ,0 };
	int CSVID = 0;
	int type = 0;
	int attr = 10;
	void MapObject_set(int a, int b) { type = a; attr = b; }
};


#endif // !_MAPOBJECT_INFO_H_
