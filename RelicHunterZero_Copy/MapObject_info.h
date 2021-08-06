#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"


class MapObject_info
{
public:
	bool in_use = false;
	bool is_ani = false;
	int max_frame = 1;
	int on_obj = 0; // 0 = empty, 1 = OBSTACLE, 2 = ITEM, 3 = enemy
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12};			// attr
	POINT pos = { 0 ,0 };
	int col = 0 ;
	int row = 0;
	int object_type = 0;
	int attr = 10;
	int rotation = 0;
	//
	MapObject_info() {}
	MapObject_info(int n) { object_type = n; }
	virtual void reset()
	{
		in_use = false;
		is_ani = false;
		max_frame = 1;
		on_obj = 0;
		pos = { 0, 0 };
		col = 0;
		row = 0;
		rotation = 0;
		attr = 10;
	}
};


#endif // !_MAPOBJECT_INFO_H_
