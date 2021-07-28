#pragma once
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include "framework.h"
#include "ScreenManager.h"
#include "Filesystem.h"


class GameManager
{
private:
	POINT rect = { 1920, 1180 };
	ULONG_PTR g_GdiToken;
	int Screen_flag = 1;
	POINT mouse;
	// Manager Object
	ScreenManager screenmanager;
	Filesystem filemanager;
public:
	//singleton
	GameManager() {}
	GameManager(const GameManager& ref) {}
	GameManager& operator=(const GameManager& ref) {}
	~GameManager() {}
	static GameManager& getInstance() { static GameManager GM;return GM; }
	// return
	int getScreen_flag() { return Screen_flag; }
	POINT getrect() { return rect; }
	POINT getmouse() { return mouse; }
	//
	void Init();
	void ShutDown();
	void GDI_Init();
	void GDI_ShutDown();
	void update();
	void show(HWND hWnd, HDC hdc);
	// manager Object
	ScreenManager getScreenmanger() { return screenmanager; }
	Filesystem getfilemanager() { return filemanager; }
};


#endif //!_GAME_MANAGER_H_

