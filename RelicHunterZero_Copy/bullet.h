#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_

#include "framework.h"

class Bullet
{
public:
	bool in_use = false;
	enum {LIGHT, NORMAL, HEAVY, SNIPER, ENEMY, 
	//state
			SHOOT, DESTROY};
	int state = SHOOT;
	POINT bulletmapoutpoint = { 0,0 };
	POINT cur = { 0,0 };
	POINT pre = { 0,0 };
	POINTF dir = { 1.0, 1.0 };
	int kind = 0;
	int gun_type = 0;
	const int col = 42;
	int row = 0;
	int rotation = 0;
	void set();




};


#endif // _BULLET_H_
