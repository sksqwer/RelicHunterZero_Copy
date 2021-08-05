#pragma once
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "framework.h"
#include "MapObject_info.h"

class obstacle_info : public MapObject_info
{
public:
	enum { STATIC = 20, ANIMATION = 21 };
	enum { BOX = 30, ROOT = 31, CAGE = 32 };
	int obstacle_type = 0;
	int kind = 10;
	int framelimit = 0;
};

#endif // !_OBSTACLE_H_
