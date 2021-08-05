#pragma once
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "framework.h"
#include "MapObject_info.h"

class obstacle_info : public MapObject_info
{
public:
	enum { STATIC = 20, ANIMATION = 21 }; // obstacle_type
	enum { BOX = 30, ROOT = 31, CAGE = 32 }; // kind
	int obstacle_type = 20;
	int kind = 30;
	int framelimit = 0;
};

#endif // !_OBSTACLE_H_
