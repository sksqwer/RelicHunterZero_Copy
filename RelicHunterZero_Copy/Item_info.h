#pragma once
#ifndef _ITEM_INFO_H_
#define _ITEM_INFO_H_

#include "framework.h"

class item_info
{
public:
	enum { GUN = 1, AMMO = 2, RECOVERY = 3 };
	int kind = 1;
	POINT pos = { 0, 0 };
	int CSVrow = 0;
	int CSVcol = 0;
	int rotation = 0;
};

#endif // !_ITEM_INFO_H_
