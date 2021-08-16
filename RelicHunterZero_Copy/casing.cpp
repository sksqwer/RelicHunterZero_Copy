#include "casing.h"

void Casing::set(POINTF p, POINTF d, int pz, int mpz, int t)
{
	pos = p;
	dir = d;
	pos_z = pz;
	max_pos_z = mpz;
	rotation = rand() % 36 * 10;
	type = t;
	gravity = 1;
	power = 10;
	switch (type)
	{
	case (LIGHT):
		col = 54;
		row = 2;
		break;
	case (MEDIUM):
		col = 54;
		row = 1;
		break;
	case (HEAVY):
		col = 54;
		row = 0;
		break;
	}
}
