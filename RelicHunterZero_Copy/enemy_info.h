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


#endif // !_ENEMY_INFO_H_
