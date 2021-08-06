#include "MapManager.h"
#include "GameManager.h"
#include "obstacle_info.h"
#include "Item_info.h"
#include "enemy_info.h"
#include "framework.h"
#include <time.h>

void MapManager::Edit_init()
{
	Map_Layer_1 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_1[i] = new MapObject_info[mapsize];

	Map_Layer_2 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_2[i] = new MapObject_info[mapsize];

	submap = new MapObject_info**[9];

	for (int i = 0; i < 9; i++)
	{
		submap[i] = new MapObject_info*[6];
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			submap[i][j] = &tile_temp[i][j];
		}
	}

	mapobject_select = new MapObject_info;

	void LoadImage();

}

void MapManager::Edit_shutdown()
{
	for (int i = 0; i < mapsize; i++)
		delete[] Map_Layer_1[i];

	delete[] Map_Layer_1;

	for (int i = 0; i < mapsize; i++)
		delete[] Map_Layer_2[i];

	delete[] Map_Layer_2;

	for (int i = 0; i < 9; i++)
	{
		delete[] submap[i];
	}

	delete[] submap;

	if(mapobject_select)
		delete mapobject_select;
}

void MapManager::TotalView()
{
	HDC hdc = GetDC(g_hWnd);
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x, by = size.y;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	


	Graphics g(hMemDC);
	g.DrawImage((GameManager::getInstance().menu)[0], 0, 0);

	Pen pen(Color(255, 0, 255, 0), 5);
	g.DrawLine(&pen, 1000, 0, 1000, 1180);

	MainView(hMemDC);

	SubView(hMemDC);

	ScreenManager::getInstance().SetCursor(hMemDC, 0, 0, 0);
	MapCursor_temp(hMemDC);

	BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);

	if (Inputsystem::getInstance().key_Q)
		(*mapobject_select).rotation -= 90;

	if (Inputsystem::getInstance().key_E)
		(*mapobject_select).rotation += 90;

	(*mapobject_select).rotation %= 360;

	ReleaseDC(g_hWnd, hdc);
}

void MapManager::MainView(HDC hdc)
{
	POINT size = GameManager::getInstance().getsize();
	int bx = 1000, by = size.y;
	POINT mouse = GameManager::getInstance().getmouse();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);

	Graphics g(hMemDC);


	Pen pen(Color(255, 0, 0, 0), 1);
	Gdiplus::SolidBrush  brush(Color(255, 255, 255, 255));
	g.FillRectangle(&brush, 0, 50, 1000, 1000);
	for (int i = 0; i < 10; i++)
	{
			g.DrawLine(&pen, 100 * i, 50, 100 * i, 1050);
			g.DrawLine(&pen, 0, 100 * i + 50, 1000, 100 * i + 50);
	}


	for (int i = mappoint.x; i < mappoint.x + 10; i++)
	{
		for (int j = mappoint.y; j < mappoint.y + 10; j++)
		{
			if (Map_Layer_1[i][j].attr == MapObject_info::BLOCK)
			{
				Pen redpen(Color(255, 255, 0, 0), 5);
				g.DrawRectangle(&redpen, Rect(100 * (i - mappoint.x), 50 + 100 * (j - mappoint.y), 100, 100));
			}
			if (Map_Layer_1[i][j].in_use)
			{
				LoadTile(hMemDC, &Map_Layer_1[i][j], 100 * ( i - mappoint.x), 50 + 100 * (j - mappoint.y));
				
			}
		}
	}
	for (int i = mappoint.x; i < mappoint.x + 10; i++)
	{
		for (int j = mappoint.y; j < mappoint.y + 10; j++)
		{
			if (Map_Layer_2[i][j].in_use)
			{
				LoadTile(hMemDC, &Map_Layer_2[i][j], 100 * (i - mappoint.x), 50 + 100 * (j - mappoint.y));

			}
			//LOAD OBSTACLE ITEM ENEMY

			Pen yellowpen(Color(255, 255, 0, 255), 3);

			static POINT premouse;
			static POINT preij;
			static bool clicked = false;
			if (mouse.x >= 100 * (i - mappoint.x) && mouse.x <= 100 + 100 * (i - mappoint.x) &&
				mouse.y >= 50 + 100 * (j - mappoint.y) && mouse.y <= 150 + 100 * (j - mappoint.y))
			{
				g.DrawRectangle(&yellowpen, Rect(100 * (i - mappoint.x), 50 + 100 * (j - mappoint.y), 100, 100));
				if (Inputsystem::getInstance().mou_L)
				{
					if ((*mapobject_select).attr == MapObject_info::SHADOW)
					{
						//LAYER2¿¡ SHADOW°´Ã¼ Ãß°¡
						Map_Layer_2[mappoint.x + i][mappoint.y + j] = (*mapobject_select);
						Map_Layer_2[mappoint.x + i][mappoint.y + j].attr = (*mapobject_select).attr;
						Map_Layer_2[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_select).object_type;
						Map_Layer_2[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
					}
					else if ((*mapobject_select).object_type == MapObject_info::OBSTACLE)
					{
						if (Map_Layer_1[mappoint.x + i][mappoint.y + j].on_obj == 0)
						{
							obstacle_info *temp = static_cast<obstacle_info *>(mapobject_select);
							(*temp).pos = { mappoint.x + i,mappoint.y + j };
							//					Map_Layer_1[mappoint.x + i][mappoint.y + j].in_use = true;
							for (int m = 0; m < temp->block.x; m++)
								for (int n = 0; n < temp->block.y; n++)
								{
									Map_Layer_1[mappoint.x + i + m][mappoint.y + j - n].attr = MapObject_info::BLOCK;
									Map_Layer_1[mappoint.x + i + m][mappoint.y + j - n].on_obj = temp->on_obj;

								}
							GameManager::getInstance().Vobstacle.push_back(*temp);
						}
						

					}
					else if ((*mapobject_select).object_type == MapObject_info::ITEM)
					{
						if (Map_Layer_1[mappoint.x + i][mappoint.y + j].on_obj == 0 )
						{
							item_info *temp = static_cast<item_info *>(mapobject_select);
							Map_Layer_1[mappoint.x + i][mappoint.y + j].on_obj = temp->on_obj;
							(*temp).pos = { mappoint.x + i,mappoint.y + j };

							GameManager::getInstance().Vitem.push_back(*temp);
						}
						else if(Map_Layer_1[mappoint.x + i][mappoint.y + j].on_obj == 1)
						{
							item_info *temp = static_cast<item_info *>(mapobject_select);
							(*temp).pos = { mappoint.x + i,mappoint.y + j };

							GameManager::getInstance().Vitem.push_back(*temp);
						}
						

					}
					else if ((*mapobject_select).object_type == MapObject_info::ENEMY)
					{
						if (Map_Layer_1[mappoint.x + i][mappoint.y + j].on_obj == 0)
						{
							enemy_info *temp = static_cast<enemy_info *>(mapobject_select);
							(*temp).pos = { mappoint.x + i,mappoint.y + j };
							//					Map_Layer_1[mappoint.x + i][mappoint.y + j].in_use = true;
							for (int m = 0; m < temp->block.x; m++)
								for (int n = 0; n < temp->block.y; n++)
								{
									Map_Layer_1[mappoint.x + i + m][mappoint.y + j - n].attr = MapObject_info::BLOCK;
									Map_Layer_1[mappoint.x + i + m][mappoint.y + j - n].on_obj = temp->on_obj;

								}
							GameManager::getInstance().Venemy.push_back(*temp);
						}
						
					}
					else
					{
						if (!clicked)
						{
							preij.x = (i - mappoint.x);
							preij.y = (j - mappoint.y);
							premouse = mouse;
						}
						Pen yellowpen(Color(255, 255, 255, 0), 5);
						g.DrawRectangle(&yellowpen, preij.x * 100, 50 + preij.y * 100, ((i - mappoint.x) - preij.x + 1) * 100, ((j - mappoint.y) - preij.y + 1) * 100);
						clicked = true;
					}
				}
				else
				{
					if (clicked)
					{
						int minx, maxx, miny, maxy;
						preij.x > (i - mappoint.x) ? (minx = preij.x, maxx = (i - mappoint.x)) : (maxx = (i - mappoint.x), minx = preij.x);
						preij.y > (j - mappoint.y) ? (miny = preij.y, maxy = (j - mappoint.y)) : (maxy = (j - mappoint.y), miny = preij.y);

						if (Inputsystem::getInstance().key_CTRL)
						{

							static int skip = 0;

							for (int i = minx; i <= maxx; i++)
							{
								if (skip == 0) skip = 1;
								else skip = 0;
								for (int j = miny; j <= maxy; j++)
								{
									if (j % 2 == skip)
										continue;

									Map_Layer_1[mappoint.x + i][mappoint.y + j] = (*mapobject_select);
									Map_Layer_1[mappoint.x + i][mappoint.y + j].attr = (*mapobject_select).attr;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_select).object_type;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
							}

						}
						else
						{
							for (int i = minx; i <= maxx; i++)
								for (int j = miny; j <= maxy; j++)
								{
									Map_Layer_1[mappoint.x + i][mappoint.y + j] = (*mapobject_select);
									Map_Layer_1[mappoint.x + i][mappoint.y + j].attr = (*mapobject_select).attr;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_select).object_type;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
						}


					}
					clicked = false;
				}

				if (Inputsystem::getInstance().mou_R)
				{
					Map_Layer_1[i][j].reset();
					Map_Layer_2[i][j].reset();

				}
			}
		}
	}
	//Obstacle
	for (int i = 0; i < GameManager::getInstance().Vobstacle.size(); i++)
	{
		obstacle_info *temp = &GameManager::getInstance().Vobstacle[i];
		LoadTile(hMemDC, temp, 100 * ((*temp).pos.x - mappoint.x), 50 + 100 * ((*temp).pos.y - mappoint.y));
	}

	//item
	for (int i = 0; i < GameManager::getInstance().Vitem.size(); i++)
	{
		item_info *temp = &GameManager::getInstance().Vitem[i];
		if(Map_Layer_1[(*temp).pos.x - mappoint.x][(*temp).pos.y - mappoint.y].on_obj == 1)
			LoadTile(hMemDC, temp, 100 * ((*temp).pos.x - mappoint.x), 100 * ((*temp).pos.y - mappoint.y));
		else
			LoadTile(hMemDC, temp, 100 * ((*temp).pos.x - mappoint.x), 50 + 100 * ((*temp).pos.y - mappoint.y));
	}

	//enemy
	for (int i = 0; i < GameManager::getInstance().Venemy.size(); i++)
	{
		enemy_info *temp = &GameManager::getInstance().Venemy[i];
		LoadTile(hMemDC, temp, 100 * ((*temp).pos.x - mappoint.x), 50 + 100 * ((*temp).pos.y - mappoint.y));

	}

	if (Inputsystem::getInstance().key_W)
	{
		mappoint.y--;
		if (mappoint.y <= 0)
			mappoint.y = 0;
	}
	if (Inputsystem::getInstance().key_S)
	{
		mappoint.y++;
		if (mappoint.y >= 40)
			mappoint.y = 39;
	}
	if (Inputsystem::getInstance().key_A)
	{
		mappoint.x--;
		if (mappoint.x <= 0)
			mappoint.x = 0;
	}
	if (Inputsystem::getInstance().key_D)
	{
		mappoint.x++;
		if (mappoint.x >= 40)
			mappoint.x = 39;
	}




	BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
}

void MapManager::SubView(HDC hdc)
{
	MapObjectView(hdc);

	int msglen;
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x - 1000, by = size.y;

	Graphics g(hdc);

	POINT mouse = GameManager::getInstance().getmouse();

	const int button_num = 14;

	//int coor[button_num][2] = { {1050, 700}, {1250, 700}, {1450, 700}, {1650, 700}, {1050, 800}, {1250, 800}, {1450, 800}, {1650, 800},
	//					{1450, 900}, {1450, 1000},{1450, 1100}, {1650, 900}, {1650, 1000}, {1650, 1100} };
	int coor[button_num][2] = { {1050, 700}, {1250, 700}, {1450, 700}, {1650, 700}, {1050, 800}, {1250, 800}, {1450, 800}, {1650, 800},
					{1450, 900}, {1450, 950},{1450, 1000}, {1650, 900}, {1650, 950}, {1650, 1000} };

	TCHAR tstr[button_num][100] = { {L"Ship"}, {L"Desert"}, {L"Green"}, {L"Volcano"}, {L"Object_Obstacle"}, {L"Object_Weapon"}, {L"Object_Enemy"}, {L"Object_ETC"},
	{L"ROAD"}, {L"BLOCK"},{L"SHADOW"}, {L"SAVE"}, {L"LOAD"}, {L"EXIT"} };

	for (int i = 0; i < button_num; i++)
	{
		if (mouse.x >= coor[i][0] && mouse.x <= int(coor[i][0] + 200) && mouse.y >= coor[i][1] && mouse.y <= int(coor[i][1] + 40) && Inputsystem::getInstance().mou_L)
		{
			g.DrawImage((GameManager::getInstance().maptoggle)[1], coor[i][0], coor[i][1]);
	
			switch (i)
			{
			case 0:
				map_flag = 0;
				break;
			case 1:
				map_flag = 1;
				break;
			case 2:
				map_flag = 2;
				break;
			case 3:
				map_flag = 3;
				break;
			case 4:
				map_flag = 4;
				break;
			case 5:
				map_flag = 5;
				break;
			case 6:
				map_flag = 6;
				break;
			case 7:
				map_flag = 7;
				break;
			case 8://ROAD
				(*mapobject_select).attr = MapObject_info::ROAD;
				break;
			case 9://BLOCK
				(*mapobject_select).attr = MapObject_info::BLOCK;
				break;
			case 10://SHADOW
				(*mapobject_select).attr = MapObject_info::SHADOW;
				break;
			case 11://SAVE
				save_map();
			break;
			case 12://LOAD
				load_map();
			break;
			case 13://EXIT
				GameManager::getInstance().setScreenflag(1);
			break;
			}


		}
		else
		{
			if (i == 8 || i == 9 || i == 10)
			{
				if ((*mapobject_select).attr == MapObject_info::ROAD && i == 8)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if((*mapobject_select).attr == MapObject_info::BLOCK && i == 9)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if((*mapobject_select).attr == MapObject_info::SHADOW && i == 10)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[0], coor[i][0], coor[i][1]);
				}

			}
			else
				g.DrawImage((GameManager::getInstance().maptoggle)[0], coor[i][0], coor[i][1]);
		}
		ScreenManager::getInstance().DoubleBufferingtext(hdc, tstr[i], coor[i][0] + 30, coor[i][1] + 5, 100, 40, 20, Color(255, 0, 0, 0));
	}

}

void MapManager::MapObjectView(HDC hdc)
{
	Graphics g(hdc);

	Gdiplus::SolidBrush  brush(Color(255, 200, 200, 200));
	g.FillRectangle(&brush, 1000, 0, 900, 600);

	POINT mouse = GameManager::getInstance().getmouse();

	static int pre_col = map_flag;
	if (pre_col != map_flag)
	{
		pre_col = map_flag;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 6; j++)
				(*submap[i][j]).reset();
	}

	switch (map_flag)
	{
	case 0:
		setship();
		break;
	case 1:
		setdesert();
		break;
	case 2:
		setgreen();
		break;
	case 3:
		setvolcano();
		break;
	case 4:
		setobstacle();
		break;
	case 5:
		setitem();
		break;
	case 6:
		setenemy();
		break; 
	case 7:
		setetc();
		break;
	}
	
	Pen blackpen(Color(255, 0, 0, 0), 1);
	for (int i = 0; i < 10; i++)
	{
		g.DrawLine(&blackpen, 1000 + 100 * i, 0, 1000 + 100 * i, 600);
	}

	for (int i = 0; i < 7; i++)
	{
		g.DrawLine(&blackpen, 1000, 100 * i, 1900, 100 * i);
	}

	

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if ((*submap[i][j]).in_use)
			{
				LoadTile(hdc, submap[i][j], 1000 + 100 * i, 100 * j);

				Pen yellowpen(Color(255, 255, 0, 255), 3);
				if (mouse.x >= 1000 + 100 * i && mouse.x <= 1100 + 100 * i &&
					mouse.y >= 100 * j && mouse.y <= 100 + 100 * j)
				{
					g.DrawRectangle(&yellowpen, Rect(1000 + 100 * i, 100 * j, 100, 100));
					if (Inputsystem::getInstance().mou_L)
					{
						submappoint.x = i;
						submappoint.y = j; 

						mapobject_select = submap[i][j];

					}
				}
			}
		}
	}
	Pen redpen(Color(255, 255, 0, 0), 5);
	g.DrawRectangle(&redpen, Rect(1000 + 100 * submappoint.x, 100 * submappoint.y, 100, 100));
}

void MapManager::MapCursor_temp(HDC hdc)
{
	if ((*mapobject_select).in_use)
	{
		POINT mouse = GameManager::getInstance().getmouse();
		LoadTile(hdc, mapobject_select, mouse.x + 50, mouse.y + 50);
	}
}

void MapManager::LoadTile(HDC hdc, MapObject_info *map, int i, int j)
{
	Graphics g(hdc);
	//if ((*map).row == -1)
	//{
	//	if ((*map).attr == MapObject_info::BLOCK)
	//	{
	//		Pen redpen(Color(255, 255, 0, 0), 5);
	//		g.DrawRectangle(&redpen, Rect(i, j, 100, 100));
	//	}
	//	return;
	//}



	Matrix mat;
	mat.RotateAt((*map).rotation % 360,
		Gdiplus::PointF(i + 50,
			j  + 50));

	g.SetTransform(&mat);
	Image *temp = returnimagepointer((*map).col, (*map).row);

	Rect rect(i, j - (temp->GetHeight() - 100), temp->GetWidth(), temp->GetHeight());
	g.DrawImage(temp, rect,
		0, 0,
		temp->GetWidth(), temp->GetHeight(), UnitPixel);

	if ((*map).is_ani)
	{
//		static int frame = 0;
//		frame %= 2;
		if (1)
		{
			(*map).row++;
			(*map).row %= (*map).max_frame;
		}
//		frame++;
	}

	if ((*map).attr == MapObject_info::BLOCK)
	{
		Pen redpen(Color(255, 255, 0, 0), 5);
		g.DrawRectangle(&redpen, Rect(i, j, 100, 100));
	}

}


void MapManager::setship()
{
	if ((*submap[0][0]).object_type != MapObject_info::TILE)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = &tile_temp[i][j];
				submap[i][j]->col = 2;
				submap[i][j]->on_obj = 0;
				submap[i][j]->object_type = MapObject_info::TILE;

			}
		}
	}
	
	//		const int csvrow = 2;
	//		const int csvcol = 0;
	//		const int filenum = 7; 	
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 2;
			(*submap[j][i]).row = a++;
			(*submap[j][i]).object_type = MapObject_info::TILE;
		}
	}

	for(int i = 4; i < 6; i++)
		for (int j = 5; j < 9; j++)
		{
			if (a == 28) break;
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 2;
			(*submap[j][i]).row = a++;
			(*submap[j][i]).object_type = MapObject_info::TILE;
		}


}

void MapManager::setdesert()
{
	if((*submap[0][0]).object_type != MapObject_info::TILE)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = &tile_temp[i][j];
				submap[i][j]->col = 3;
				submap[i][j]->on_obj = 0;
				submap[i][j]->object_type = MapObject_info::TILE;

			}
		}
	}
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 3;
			(*submap[j][i]).row = a++;
			(*submap[j][i]).object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		(*submap[0][i]).in_use = true;
		(*submap[0][i]).col = 3;
		(*submap[0][i]).row = a++;
		(*submap[0][i]).object_type = MapObject_info::TILE;
	}

	for (int i = 1; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 3;
			(*submap[j][i]).row = a++;

			(*submap[j][i]).object_type = MapObject_info::SHADOW;
		}
	}



}

void MapManager::setgreen()
{
	if ((*submap[0][0]).object_type != MapObject_info::TILE)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = &tile_temp[i][j];
				submap[i][j]->col = 4;
				submap[i][j]->on_obj = 0;
				submap[i][j]->object_type = MapObject_info::TILE;

			}
		}
	}
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 4;
			(*submap[j][i]).row = a++;
			(*submap[j][i]).object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		(*submap[0][i]).in_use = true;
		(*submap[0][i]).col = 4;
		(*submap[0][i]).row = a++;
		(*submap[0][i]).object_type = MapObject_info::TILE;
	}

	for (int i = 1; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 4;
			(*submap[j][i]).row = a++;

			(*submap[j][i]).object_type = MapObject_info::SHADOW;
		}
	}

}

void MapManager::setvolcano()
{
	if ((*submap[0][0]).object_type != MapObject_info::TILE)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = &tile_temp[i][j];
				submap[i][j]->col = 5;
				submap[i][j]->on_obj = 0;
				submap[i][j]->object_type = MapObject_info::TILE;

			}
		}
	}
	
	int a = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 5;
			(*submap[j][i]).row = a++;
			(*submap[j][i]).object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		(*submap[0][i]).in_use = true;
		(*submap[0][i]).col = 5;
		(*submap[0][i]).row = a++;
		(*submap[0][i]).object_type = MapObject_info::TILE;
	}

	for (int i = 1; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			(*submap[j][i]).in_use = true;
			(*submap[j][i]).col = 5;
			(*submap[j][i]).row = a++;

			(*submap[j][i]).object_type = MapObject_info::SHADOW;
		}
	}
}

void MapManager::setobstacle()
{
	if ((*submap[0][0]).object_type != MapObject_info::OBSTACLE)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = (dynamic_cast<obstacle_info *>(&obstacle_temp[i][j]));
				submap[i][j]->col = 6;
				submap[i][j]->on_obj = 1;
				submap[i][j]->object_type = MapObject_info::OBSTACLE;
			}
		}

	int a = 0;

	for (int i = 1; i < 5; i += 2)
	{
		obstacle_temp[0][i].in_use = true;
		obstacle_temp[0][i].on_obj = 1;
		obstacle_temp[0][i].col = 6;
		obstacle_temp[0][i].row = a++;

		obstacle_temp[0][i].obstacle_type = obstacle_info::STATIC;
		obstacle_temp[0][i].kind = obstacle_info::BOX;
	}
	obstacle_temp[0][5].in_use = true;
	obstacle_temp[0][5].on_obj = 1;
	obstacle_temp[0][5].col = 6;
	obstacle_temp[0][5].row = a++;

	obstacle_temp[0][5].obstacle_type = obstacle_info::STATIC;
	obstacle_temp[0][5].kind = obstacle_info::BOX;
	obstacle_temp[0][5].block = {2, 1};

	for (int i = 1; i < 5; i += 2)
	{
		obstacle_temp[2][i].in_use = true;
		obstacle_temp[2][i].on_obj = 1;
		obstacle_temp[2][i].col = 6;
		obstacle_temp[2][i].row = a++;

		obstacle_temp[2][i].obstacle_type = obstacle_info::STATIC;
		obstacle_temp[2][i].kind = obstacle_info::ROOT;
		obstacle_temp[2][i].block = { 2, 1 };
	}

	obstacle_temp[5][1].in_use = true;
	obstacle_temp[5][1].on_obj = 1;
	obstacle_temp[5][1].col = 6;
	obstacle_temp[5][1].row = a++;

	obstacle_temp[5][1].obstacle_type = obstacle_info::STATIC;
	obstacle_temp[5][1].kind = obstacle_info::ROOT;
	obstacle_temp[5][1].block = { 3, 2 };
	setcage();
	}
}

void MapManager::setcage()
{
	int a = 0;
	//bool is_set = false;
	//if (!is_set)
	//{
	//	obstacle_temp[5][3].row = a;
	//}
	obstacle_temp[5][3].row = a;

	obstacle_temp[5][3].in_use = true;
	obstacle_temp[5][3].col = 7;

	obstacle_temp[5][3].object_type = MapObject_info::OBSTACLE;
	obstacle_temp[5][3].framelimit = 12;
	obstacle_temp[5][3].max_frame = 12;
	obstacle_temp[5][3].on_obj = 1;
	obstacle_temp[5][3].is_ani = true;
	obstacle_temp[5][3].block = { 2, 2 };


	obstacle_temp[5][3].obstacle_type = obstacle_info::ANIMATION;
	obstacle_temp[5][3].kind = obstacle_info::CAGE;
}

void MapManager::setitem()
{
	if ((*submap[0][0]).object_type != MapObject_info::ITEM)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = (dynamic_cast<item_info *>(&item_temp[i][j]));
				submap[i][j]->col = 8;
				submap[i][j]->on_obj = 2;
				submap[i][j]->object_type = MapObject_info::ITEM;
			}
		}
		int a = 0;
		int b = item_info::PISTOL;

		for (int j = 0; j < 6; j++)
		{
			item_temp[0][j].in_use = true;
			item_temp[0][j].on_obj = 2;
			item_temp[0][j].kind = b++;
			item_temp[0][j].row = a++;
		}

		for (int i = 0; i < 4; i++)
		{
			item_temp[1][i].in_use = true;
			item_temp[1][i].on_obj = 2;
			item_temp[1][i].kind = b++;
			item_temp[1][i].row = a++;
		}

		for (int i = 0; i < 3; i++)
		{
			item_temp[2][i].in_use = true;
			item_temp[2][i].on_obj = 2;
			item_temp[2][i].kind = b++;
			item_temp[2][i].row = a++;
		}

	}
}

void MapManager::setenemy()
{
	if ((*submap[0][0]).object_type != MapObject_info::ENEMY)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				submap[i][j] = (dynamic_cast<enemy_info *>(&enemy_temp[i][j]));
				submap[i][j]->on_obj = 3;
				submap[i][j]->object_type = MapObject_info::ENEMY;
			}
		}
		int a = 9;
		int b = enemy_info::DUCK;

		for (int i = 0; i < 3; i++)
		{
			enemy_temp[i][0].in_use = true;
			enemy_temp[i][0].on_obj = 3;
			enemy_temp[i][0].kind = b++;
			enemy_temp[i][0].col = a++;
			enemy_temp[i][0].row = 0;
			enemy_temp[i][0].is_ani = true;
			enemy_temp[i][0].framelimit = 7;
			enemy_temp[i][0].max_frame = 7;
		}
		enemy_temp[2][0].framelimit = 10;
		enemy_temp[2][0].max_frame = 10;
		for (int i = 0; i < 2; i++)
		{
			enemy_temp[i][1].in_use = true;
			enemy_temp[i][1].on_obj = 3;
			enemy_temp[i][1].kind = b++;
			enemy_temp[i][1].col = a++;
			enemy_temp[i][1].row = 0;
			enemy_temp[i][1].is_ani = true;
			enemy_temp[i][1].framelimit = 16;
			enemy_temp[i][1].max_frame = 16;
		}

		for (int i = 0; i < 3; i++)
		{
			enemy_temp[i][2].in_use = true;
			enemy_temp[i][2].on_obj = 3;
			enemy_temp[i][2].kind = b++;
			enemy_temp[i][2].col = a++;
			enemy_temp[i][2].row = 0;
			enemy_temp[i][2].is_ani = true;
			enemy_temp[i][2].framelimit = 12;
			enemy_temp[i][2].max_frame = 12;
		}
		enemy_temp[2][2].framelimit = 14;
		enemy_temp[2][2].max_frame = 14;

		//boss
		enemy_temp[4][3].in_use = true;
		enemy_temp[4][3].on_obj = 3;
		enemy_temp[4][3].kind = b++;
		enemy_temp[4][3].col = a++;
		enemy_temp[4][3].row = 0;
		enemy_temp[4][3].is_ani = true;
		enemy_temp[4][3].block = { 2, 1 };
		enemy_temp[4][3].framelimit = 14;
		enemy_temp[4][3].max_frame = 14;

	}
}

void MapManager::setetc()
{
}

Image *MapManager::returnimagepointer(int n, int m)
{
	switch (n)
	{
	case 0:
		return (GameManager::getInstance().menu)[m];
	case 1:
		return (GameManager::getInstance().maptoggle)[m];
	case 2:
		return (GameManager::getInstance().ship)[m];
	case 3:
		return (GameManager::getInstance().desert)[m];
	case 4:
		return (GameManager::getInstance().green)[m];
	case 5:
		return (GameManager::getInstance().volcano)[m];
	case 6:
		return (GameManager::getInstance().obstacle)[m];
	case 7:
		return (GameManager::getInstance().obstacle_cage)[m];
	case 8:
		return (GameManager::getInstance().item)[m];
	case 9:
		return (GameManager::getInstance().duck1_idle)[m];
	case 10:
		return (GameManager::getInstance().duck2_idle)[m];
	case 11:
		return (GameManager::getInstance().duck3_idle)[m];
	case 12:
		return (GameManager::getInstance().kamikaze1_idle)[m];
	case 13:
		return (GameManager::getInstance().kamikaze2_idle)[m];
	case 14:
		return (GameManager::getInstance().turtle1_idle)[m];
	case 15:
		return (GameManager::getInstance().turtle2_idle)[m];
	case 16:
		return (GameManager::getInstance().turtle3_idle)[m];
	case 17:
		return (GameManager::getInstance().boss_idle)[m];

	}
	


	return nullptr;
}

void MapManager::save_map()
{
	TCHAR szFile[100];

	OPENFILENAME ofn;


	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
}

void MapManager::load_map()
{

}

