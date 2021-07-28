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
	void Screen(HWND hWnd, HDC hdc);
	void MainMenu(HWND hWnd, HDC hdc);
	void DoubleBufferingimage(HWND hWnd, HDC hdc, Image *, int x, int y);
	void DoubleBufferingtile(HWND hWnd, HDC hdc, Image *, int x, int y, int cx, int  cy);
	void DoubleBufferingtext(HWND hWnd, HDC hdc, const TCHAR *, int x, int y, int r, int b);
};



#endif //!_SCREEN_MANAGER_H_