#pragma once
#ifndef _ENEMY_INFO_H_
#define _ENEMY_INFO_H_

#include "framework.h"
#include "MapObject_info.h"

class enemy_info : public MapObject_info
{
public:
	enum { DUCK = 51, DUCK2= 52, DUCK3 = 53, KAMIKAZE1 = 54, KAMIKAZE2 = 55,
			TURTLE1 = 56, TURTLE2 = 57, TURTLE3 = 58, BOSS = 59};
	int kind = 51;
	POINT pos = { 0, 0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int rotation = 0;
};


#endif // !_ENEMY_INFO_H_
