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
	Image *obstacle_cage[12] = { nullptr };
	Image *item[13] = { nullptr };
	Image *duck1_idle[7] = { nullptr };
	Image *duck2_idle[7] = { nullptr };
	Image *duck3_idle[10] = { nullptr };
	Image *kamikaze1_idle[16] = { nullptr };
	Image *kamikaze2_idle[16] = { nullptr };
	Image *turtle1_idle[12] = { nullptr };
	Image *turtle2_idle[12] = { nullptr };
	Image *turtle3_idle[14] = { nullptr };
	Image *boss_idle[14] = { nullptr };
	Image *char_sel_toggle[9] = { nullptr };
	Image *char_sel_unselected[5] = { nullptr };
	Image *char_sel_stat[5] = { nullptr };
	Image *char_sel_jimmy[3] = { nullptr };
	Image *char_sel_jimmy_sel[20] = { nullptr };
	Image *bigwindow[60] = { nullptr };
	Image *etc[8] = { nullptr };
	Image *bigwindow_reflection[60] = { nullptr };
	Image *holo_Jimmy[27] = { nullptr };
	Image *holo_Pinky[27] = { nullptr };
	Image *holo_Biu[27] = { nullptr };
	Image *holo_Punny[27] = { nullptr };
	Image *holo_Raff[27] = { nullptr };
	Image *Keytutorial[26] = { nullptr };
	Image *NPC_ass[10] = { nullptr };
	// objectvector


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
	void show_FPS(HDC hdc);
	//
	void Load_Image();
	//
	void load_image_func(const int CSVrow, const int CSVcol, const int filenum, Image **img);
	void shutdown_image_func(Image **img);
	Image *returnimagepointer(int n, int m);

};


#endif //!_GAME_MANAGER_H_

