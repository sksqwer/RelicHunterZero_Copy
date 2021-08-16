#pragma once
#ifndef _MAPOBJECT_INFO_H_
#define _MAPOBJECT_INFO_H_

#include "framework.h"

class obstacle_ingame;
class enemy_ingame;
class item_ingame;

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
		object_type = 0;
		col = 0;
		row = 0;
		rotation = 0;
		attr = 10;
	}
};

class MapObject_ingame : public MapObject_info
{
public:
	obstacle_ingame *obs = NULL;
	item_ingame *item = NULL;
	enemy_ingame *enemy = NULL;

	void reset()
	{
		is_ani = false;
		max_frame = 1;
		on_obj = 0;
		object_type = 0;
		attr = 10;
		obs = NULL;
		item = NULL;
		enemy = NULL;
	}
	//function
	MapObject_ingame& operator=(const MapObject_info& mapobj);
};


#endif // !_MAPOBJECT_INFO_H_
