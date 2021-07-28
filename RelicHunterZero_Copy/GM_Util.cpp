#include "GM_Util.h"

bool is_in_rectangle(int left, int top, int right, int bottom, int x, int y)
{
	if (x >= left && x <= right && y >= top && y <= bottom)
		return true;
	else
		return false;
}

bool is_in_rectangle(int left, int top, int right, int bottom, POINT pos)
{
	if (pos.x >= left && pos.x <= right && pos.y >= top && pos.y <= bottom)
		return true;
	else
		return false;
}
