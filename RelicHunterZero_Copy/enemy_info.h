#pragma once
#ifndef _ENEMY_INFO_H_
#define _ENEMY_INFO_H_

#include "framework.h"

class enemy_info
{
public:
	enum { DUCK = 1, DUCK2= 2, DUCK3 = 3, KAMIKAZE1 = 4, KAMIKAZE2 = 5,
			TURTLE1 = 6, TURTLE2 = 7, TURTLE3 = 8, BOSS = 9};
	int kind = 1;
	POINT pos = { 0, 0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int rotation = 0;
};


#endif // !_ENEMY_INFO_H_
