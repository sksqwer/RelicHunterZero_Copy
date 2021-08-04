#pragma once
#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include "framework.h"
//#include "GameManager.h"


class GameManager;

class ScreenManager
{
private:
	int bg_tile_x = 0, bg_tile_y = 0;
public:
	//singleton
	ScreenManager() {}
	ScreenManager(const ScreenManager& ref) {}
	ScreenManager& operator=(const ScreenManager& ref) {}
	~ScreenManager() {}
	static ScreenManager& getInstance() { static ScreenManager SM; return SM; }
	//
	void Screen();
	void MainMenu();
	void DoubleBufferingimage(HDC hdc, Image *, int x, int y);
	void DoubleBufferingtile(HDC hdc, Image *, int x, int y, int cx, int  cy);
	void DoubleBufferingtext(HDC hdc, const TCHAR *, int x, int y, int r, int b, int size, Color);
	void SetCursor(HDC hdc, int x, int y, int i);
};



#endif //!_SCREEN_MANAGER_H_