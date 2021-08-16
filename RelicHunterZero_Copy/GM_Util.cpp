#define _USE_MATH_DEFINES

#include "GM_Util.h"
#include <cmath>

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

float get_distance(POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.y)
		return 0.001;
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float get_distance(POINT a, POINTF b)
{
	if (a.x == b.x && a.y == b.y)
		return 0.001;
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float angle(POINT a, POINT b)
{
	float dis = get_distance(a, b);
	float anglevalue = atan2((b.y - a.y) , (b.x - a.x));
	anglevalue = anglevalue * 180 / M_PI;

	return anglevalue;
}

POINT rotate_dot(POINT core, POINT b, int rotation)
{
	POINT temp;
	float l = get_distance(core, b);

	float radian = rotation * M_PI / 180;
	temp.x = core.x + l * cos(radian);
	temp.y = core.y + l * sin(radian);
//	temp.x = b.x * cos(radian) - b.y * sin(radian);
//	temp.y = b.x * cos(radian) + b.y * sin(radian);

	return temp;

}

POINTF rotate_dot(POINT core, POINTF b, int rotation)
{
	POINTF temp;
	float l = get_distance(core, b);

	float radian = rotation * M_PI / 180;
	//temp.x = core.x + l * cos(radian);
	//temp.y = core.y + l * sin(radian);
	temp.x = (b.x - core.x) * cos(radian) - (b.y - core.y)* sin(radian);
	temp.y = (b.x - core.x) * sin(radian) + (b.y - core.y) * cos(radian);
	temp.x += core.x;
	temp.y += core.y;

	return temp;
}

POINT& operator+(const POINT a, const POINT b)
{
	POINT temp = { a.x + b.x, a.y + b.y };
	return temp;
}

Point & operator+(const Point a, const POINT b)
{
	Point temp = { a.X + b.x, a.Y + b.y };
	return temp;
}

Point & operator+(const POINT a, const Point b)
{
	Point temp = { a.x + b.X, a.y + b.Y };
	return temp;
}




