#include "GameScreen.h"

#include "GameManager.h"
#include "Mapmanager.h"

void GameScreen::init()
{
	Map_Layer_1 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_1[i] = new MapObject_info[mapsize];

	Map_Layer_2 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_2[i] = new MapObject_info[mapsize];

	MapManager::getInstance().load_map(Map_Layer_1, Map_Layer_2, Vobstacle, Vitem, Venemy, stage1);

}

void GameScreen::shutdown()
{
	for (int i = 0; i < mapsize; i++)
		delete[] Map_Layer_1[i];

	delete[] Map_Layer_1;

	for (int i = 0; i < mapsize; i++)
		delete[] Map_Layer_2[i];

	delete[] Map_Layer_2;

	Vobstacle.clear();
	Vitem.clear();
	Venemy.clear();
}

void GameScreen::TotalGameview()
{
	HDC hdc = GetDC(g_hWnd);
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x + 400, by = size.y + 400;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);

	Graphics g(hMemDC);


	ScreenManager::getInstance().SetCursor(hMemDC, 0, 0, 0);


	MapView(hMemDC);


	if (Inputsystem::getInstance().mou_L)
		GameManager::getInstance().setScreenflag(1);

	GameManager::getInstance().show_FPS(hMemDC);

	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x, mapinpoint.y, SRCCOPY);
//	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);


	ReleaseDC(g_hWnd, hdc);
}

void GameScreen::MapView(HDC hdc)
{
	for(int i = mapoutpoint.x; i < mapoutpoint.x + 14; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 14; j++)
		{
			if (Map_Layer_1[i][j].in_use)
			{
				MapManager::getInstance().LoadTile(hdc, &Map_Layer_1[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

			}
		}

	for (int i = mapoutpoint.x; i < mapoutpoint.x + 14; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 14; j++)
		{
			if (Map_Layer_2[i][j].in_use)
			{
				MapManager::getInstance().LoadTile(hdc, &Map_Layer_2[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

			}
		}

	for (int i = 0; i < Vobstacle.size(); i++)
	{
		obstacle_info *temp = &Vobstacle[i];
		MapManager::getInstance().LoadTile(hdc, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
	}

	//item
	for (int i = 0; i < Vitem.size(); i++)
	{
		item_info *temp = &Vitem[i];
		if (Map_Layer_1[(*temp).pos.x][(*temp).pos.y].on_obj == 1)
			MapManager::getInstance().LoadTile(hdc, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 70 + 100 * ((*temp).pos.y - mapoutpoint.y), false);
		else
			MapManager::getInstance().LoadTile(hdc, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
	}

	//enemy
	for (int i = 0; i < Venemy.size(); i++)
	{
		enemy_info *temp = &Venemy[i];
		MapManager::getInstance().LoadTile(hdc, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);

	}
}
