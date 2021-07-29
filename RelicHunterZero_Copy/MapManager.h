#pragma once
#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_

#include "framework.h"
#include "MapObject_info.h"

class MapManager
{
private:
	const int mapsize = 50;
	const int totalviewsize = 5500;
	POINT mappoint = { 500, 500 };
	MapObject_info **mapinfo = NULL;
public:
	//singleton
	MapManager() {}
	MapManager(const MapManager& ref) {}
	MapManager& operator=(const MapManager& ref) {}
	~MapManager() {}
	static MapManager& getInstance() { static MapManager ME; return ME; }
	//
	void Edit_init();
	void Edit_shutdown();
	void TotalView(HWND hWnd, HDC hdc);
	void MainView(HWND hWnd, HDC hdc);
	void SubView(HWND hWnd, HDC hdc);


};



#endif // !_MAP_MANAGER_H_
