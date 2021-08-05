#pragma once
#ifndef _ITEM_INFO_H_
#define _ITEM_INFO_H_

#include "framework.h"
#include "MapObject_info.h"

class item_info : public MapObject_info
{
public:
	enum { GUN = 41, AMMO = 42, RECOVERY = 43 };
	int kind = 41;
	POINT pos = { 0, 0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int rotation = 0;
};

#endif // !_ITEM_INFO_H_
