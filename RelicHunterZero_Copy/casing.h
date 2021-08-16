#pragma once
#ifndef _CASING_H_
#define _CASING_H_

#include "framework.h"

class Casing // ÅºÇÇ
{
public:
	enum { LIGHT = 1, MEDIUM = 2, HEAVY = 3 };
	POINTF pos = { 0, 0 };
	POINTF dir = { 0, 0 };
	int pos_z = 50;
	int max_pos_z = 100;
	int rotation = 0;
	int gravity = 1;
	int power = 5;
	int type = LIGHT;
	int col = 54;
	int row = 0;
	void set(POINTF p, POINTF d, int pz, int mpz, int t);
};


#endif // !_CASING_H_
