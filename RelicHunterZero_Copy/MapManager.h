#pragma once
#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_

#include "framework.h"
#include "MapObject_info.h"
#include "obstacle_info.h"
#include "Item_info.h"
#include "enemy_info.h"

class MapManager
{
private:
	const int mapsize = 100;
	const int totalviewsize = 5500;
	POINT mappoint = { 0, 0 };
	POINT submappoint = { 0, 0 };
	POINT deletepoint = { -1, -1 };
	MapObject_info **Map_Layer_1 = NULL;
	MapObject_info **Map_Layer_2 = NULL;
	MapObject_info ***submap;
	MapObject_info *mapobject_select;
	MapObject_info tile_temp[9][6];
	obstacle_info obstacle_temp[9][6];
	item_info item_temp[9][6];
	enemy_info enemy_temp[9][6];
	int map_flag = 0;
	std::vector<obstacle_info> Vobstacle;
	std::vector<item_info> Vitem;
	std::vector<enemy_info> Venemy;



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
	void TotalView();
	void MainView(HDC hdc);
	void SubView(HDC hdc);
	void MapObjectView(HDC hdc);
	void MapCursor_temp(HDC hdc);
	//
	void LoadTile(HDC hdc, MapObject_info *map, int i, int j, bool is_mapedit);
	void LoadTile(Graphics* g, MapObject_info *map, int i, int j, bool is_mapedit);
	//set submap
	void setship();
	void setdesert();
	void setgreen();
	void setvolcano();
	void setobstacle();
	void setcage();
	void setitem();
	void setenemy();
	void setetc();
	//
	void save_map();
	void load_map(MapObject_info **, MapObject_info **, std::vector<obstacle_info>&, std::vector<item_info>&, std::vector<enemy_info>&);
	void load_map(MapObject_info **, MapObject_info **, std::vector<obstacle_info>&, std::vector<item_info>&, std::vector<enemy_info>&, const char*);

};



#endif // !_MAP_MANAGER_H_
