#pragma once
#ifndef _GM_UTIL_H_
#define _GM_UTIL_H_

#include "framework.h"

bool is_in_rectangle(int left, int top, int right, int bottom, int x, int y);
bool is_in_rectangle(int left, int top, int right, int bottom, POINT pos) {	return is_in_rectangle(left, top, right, bottom, pos.x, pos.y); }



#endif !_GM_UTIL_H_