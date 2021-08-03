#pragma once
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include "framework.h"
#include "ScreenManager.h"
#include "Filesystem.h"
#include "Inputsystem.h"


class GameManager
{
private:
	POINT size = { 1920, 1180 };
	ULONG_PTR g_GdiToken;
	int Screen_flag = 1;
	POINT mouse;
public:
	//singleton
	GameManager() {}
	GameManager(const GameManager& ref) {}
	GameManager& operator=(const GameManager& ref) { }
	~GameManager() {}
	static GameManager& getInstance() { static GameManager GM; return GM; }
	// return
	int getScreen_flag() { return Screen_flag; }
	POINT getsize() { return size; }
	POINT getmouse() { return mouse; }
	//set
	void setScreenflag(int n) { Screen_flag = n; }
	//
	void Init();
	void ShutDown();
	void GDI_Init();
	void GDI_ShutDown();
	void update();
	void show(HWND hWnd, HDC hdc);
};


#endif //!_GAME_MANAGER_H_

