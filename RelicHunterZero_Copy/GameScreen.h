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
	const int mapsize = 100;
	const char stage1[100] = "Resource/Save/stage1.txt";
	int stage = 0;
	int rotation = 0;
	POINT mapoutpoint = {3, 3 };
	POINT mapinpoint = { 200, 200 };
	POINT premapoutpoint = { 2, 2 };
	POINT premapinpoint = { 200, 200 };
	POINT mouseinpoint = { 0, 0 };
	Player *player = NULL;
	// object
	HBITMAP *mapImage;
	Image *mapImagepng;
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
	//view
	void TotalGameview();
	void mapImageview(HDC hdc, Graphics* g);
	void stage1MapView(HDC hdc, Graphics* g);
	void print_Player(Graphics* g);
	void print_bullet(Graphics* g);
	void print_Player_shadow(Graphics* g);
	void load_Image(Graphics* g, Image *img, int a, int b, bool reverse);
	void load_Image_shadow(Graphics* g, Image *img, int a, int b, bool reverse);

	//update
	void update();
	void update_bullet();

	//collide
	void collide_player();

	




};

#endif // !_GAMESCREEN_H_
