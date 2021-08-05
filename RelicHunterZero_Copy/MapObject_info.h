#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"


class MapObject_info
{
public:
	bool in_use = false;
	int on_obj = 0; // 0 = empty, 1 = BOX, 2 = OBSTACLE, 3 = ITEM, 4 = enemy
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12, OBJECT = 13};			// attr
	POINT pos = { 0 ,0 };
	int col = 0;
	int row = 0;
	int object_type = 0;
	int attr = 10;
	int imgrow = 100;
	int imgcol = 100;
	int rotation = 0;
	//
	void MapObject_reset()
	{
		in_use = false;
		on_obj = 0;
		pos = { 0, 0 };
		object_type = 0;
		imgrow = 100;
		imgcol = 100;
		attr = 10;
	}
};


#endif // !_MAPOBJECT_INFO_H_
