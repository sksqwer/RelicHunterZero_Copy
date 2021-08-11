#pragma once
#ifndef _GM_UTIL_H_
#define _GM_UTIL_H_



#include "framework.h"

bool is_in_rectangle(int left, int top, int right, int bottom, int x, int y);
bool is_in_rectangle(int left, int top, int right, int bottom, POINT pos);
float get_distance(POINT a, POINT b);
float angle(POINT a, POINT b);
POINT rotate_dot(POINT core, POINT b, int rotation);



#endif !_GM_UTIL_H_