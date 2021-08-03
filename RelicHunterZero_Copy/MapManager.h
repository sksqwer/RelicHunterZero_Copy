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
	POINT mappoint = { 0, 0 };
	POINT submappoint = { 0, 0 };
	MapObject_info **mapinfo = NULL;
	MapObject_info submap[9][6];
	MapObject_info mapobject_temp;
	int map_flag = 0;
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
	void MapObjectView(HWND hWnd, HDC hdc);
	void MapCursor_temp(HWND hWnd, HDC hdc);
	void LoadTile(HDC hdc, MapObject_info map, int i, int j);
	//set submap
	void setship();
	void setdesert();
	void setgreen();
	void setvolcano();


};



#endif // !_MAP_MANAGER_H_
