#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"


class MapObject_info
{
public:
	bool in_use = false;
	int on_obj = 0; // 0 = empty, 1 = OBSTACLE, 2 = ITEM, 3 = enemy
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12};			// attr
	POINT pos = { 0 ,0 };
	int col = -1;
	int row = -1;
	int object_type = 0;
	int attr = 10;
	int rotation = 0;
	//
	virtual void reset()
	{
		in_use = false;
		on_obj = 0;
		pos = { 0, 0 };
		col = 0;
		row = 0;
		object_type = 0;
		rotation = 0;
		attr = 10;
	}
};


#endif // !_MAPOBJECT_INFO_H_
