#pragma once
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include "framework.h"
#include "ScreenManager.h"
#include "Filesystem.h"
#include "Inputsystem.h"
#include "MapObject_info.h"
#include "obstacle_info.h"
#include "Item_info.h"
#include "enemy_info.h"


class GameManager
{
private:
	POINT size = { 1920, 1180 };
	ULONG_PTR g_GdiToken;
	int Screen_flag = 1;
	POINT mouse;
public:
	//Image
	Image *menu[6] = { nullptr };
	Image *maptoggle[3] = { nullptr };
	Image *ship[28] = { nullptr };
	Image *desert[28] = { nullptr };
	Image *green[28] = { nullptr };
	Image *volcano[28] = { nullptr };
	Image *obstacle[6] = { nullptr };
	// objectvector
	std::vector<obstacle_info> Vobstacle;
	std::vector<item_info> Vitem;
	std::vector<enemy_info> Venemy;


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
	void show();
	void Load_Image();
	//Imageload
	void load_menu();
	void load_maptoggle();
	void load_ship();
	void load_desert();
	void load_green();
	void load_vocano();
	void load_obstacle();

};


#endif //!_GAME_MANAGER_H_

