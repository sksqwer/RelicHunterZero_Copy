#pragma once
#ifndef _GAMESCREEN_H_
#define _GAMESCREEN_H_
#include "MapObject_info.h"
#include "obstacle_info.h"
#include "Item_info.h"
#include "enemy_info.h"
#include "Player.h"

class GameScreen
{
public:
	//variable
	const int mapsize = 50;
	const char stage1[100] = "Resource/Save/stage1.txt";
	int stage = 0;
	POINT mapoutpoint = {3, 3 };
	POINT mapinpoint = { 200, 200 };
	POINT mouseinpoint = { 0, 0 };
	Player *player = NULL;
	// object
	MapObject_info **Map_Layer_1 = NULL;
	MapObject_info **Map_Layer_2 = NULL;
	std::vector<obstacle_info> Vobstacle;
	std::vector<item_info> Vitem;
	std::vector<enemy_info> Venemy;
	//singleton
	GameScreen() {}
	GameScreen(const GameScreen& ref) {}
	GameScreen& operator=(const GameScreen& ref) {}
	~GameScreen() {}
	static GameScreen& getInstance() { static GameScreen SM; return SM; }
	//init
	void init();
	void shutdown();
	void set_player();
	//view
	void TotalGameview();
	void stage1MapView(HDC hdc);
	void print_Player(HDC hdc);
	void print_Player_shadow(HDC hdc);
	void load_Image(HDC hdc, Image *img, int a, int b, bool reverse);
	void load_Image_shadow(HDC hdc, Image *img, int a, int b, bool reverse);

	//update
	void update();

	




};

#endif // !_GAMESCREEN_H_
