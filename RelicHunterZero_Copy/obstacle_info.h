#pragma once
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "framework.h"

class obstacle_info
{
public:
	enum { STATIC = 0, MOVE = 1 };
	enum { BOX = 10, ROOT = 11, CAGE = 12 };
	int type = 0;
	int kind = 10;
	POINT pos = { 0, 0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int imgsizerow = 100;
	int imgsizecol = 100;
	int rotation = 0;
};

#endif // !_OBSTACLE_H_
