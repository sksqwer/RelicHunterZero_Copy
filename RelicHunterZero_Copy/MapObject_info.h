#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"

class MapObject_info
{
public:
	bool in_use = false;
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12};			// attr
	POINT pos = { 0 ,0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int imgrow = 0;
	int imgcol = 0;
	int type = 0;
	int attr = 10;
	int rotation = 0;
	MapObject_info *Shadow = NULL;
	void MapObject_set(int a, int b) { type = a; attr = b; }
	void MapObject_reset()
	{
		in_use = false;
		pos = { 0, 0 };
		CSVrow = 0;
		CSVcol = 0;
		imgrow = 0;
		imgcol = 0;
		type = 0;
		attr = 10;
		if (Shadow)
		{
			Shadow->MapObject_reset();
			delete Shadow;
			Shadow = NULL;
		}
	}
};


#endif // !_MAPOBJECT_INFO_H_
