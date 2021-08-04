#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"
#include "enemy_info.h"
#include "obstacle_info.h"
#include "item_info.h"


class MapObject_info
{
public:
	bool in_use = false;
	bool is_occupy = false;
	enum {TILE = 0, OBSTACLE = 1, ITEM = 2, ENEMY = 3}; //type
	enum {ROAD = 10, BLOCK = 11, SHADOW = 12};			// attr
	POINT pos = { 0 ,0 };
	int row = 1;
	int col = 1;
	int CSVrow = 0;
	int CSVcol = 0;
	int imgcutrow = 0;
	int imgcutcol = 0;
	int imgsizerow = 100;
	int imgsizecol = 100;
	int type = 0;
	int attr = 10;
	int rotation = 0;
	MapObject_info *Shadow = NULL;
	//temp
	obstacle_info obstacle_temp;
	item_info item_temp;
	enemy_info enemy_temp;
	//
	void MapObject_set(int a, int b) { type = a; attr = b; }
	void MapObject_reset()
	{
		in_use = false;
		is_occupy = false;
		pos = { 0, 0 };
		CSVrow = 0;
		CSVcol = 0;
		imgcutrow = 0;
		imgcutcol = 0;
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
