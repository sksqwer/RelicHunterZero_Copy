#include "MapManager.h"
#include "GameManager.h"
#include "obstacle_info.h"
#include "Item_info.h"
#include "enemy_info.h"
#include <time.h>

void MapManager::Edit_init()
{
	Map_Layer_1 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_1[i] = new MapObject_info[mapsize];

	Map_Layer_2 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_2[i] = new MapObject_info[mapsize];

	submap = new MapObject_info*[9];
	for (int i = 0; i < 9; i++)
		submap[i] = new MapObject_info[6];

	mapobject_temp = new MapObject_info;

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
		delete[] submap[i];

	delete[] submap;

	if(mapobject_temp)
		delete mapobject_temp;
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
		(*mapobject_temp).rotation -= 90;

	if (Inputsystem::getInstance().key_E)
		(*mapobject_temp).rotation += 90;

	(*mapobject_temp).rotation %= 360;

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
			if (Map_Layer_1[mappoint.x + i][mappoint.y + j].in_use)
			{
				LoadTile(hMemDC, &Map_Layer_1[mappoint.x + i][mappoint.y + j], 100 * ( i - mappoint.x), 50 + 100 * (j - mappoint.y));
				
			}

			
			

		}
	}
	for (int i = mappoint.x; i < mappoint.x + 10; i++)
	{
		for (int j = mappoint.y; j < mappoint.y + 10; j++)
		{
			if (Map_Layer_2[mappoint.x + i][mappoint.y + j].in_use)
			{
				LoadTile(hMemDC, &Map_Layer_2[mappoint.x + i][mappoint.y + j], 100 * (i - mappoint.x), 50 + 100 * (j - mappoint.y));

			}
			//LOAD OBSTACLE ITEM ENEMY

			Pen yellowpen(Color(255, 255, 0, 255), 3);

			static POINT premouse;
			static POINT preij;
			static bool clicked = false;
			if (mouse.x >= 100 * i && mouse.x <= 100 + 100 * i &&
				mouse.y >= 50 + 100 * j && mouse.y <= 150 + 100 * j)
			{
				g.DrawRectangle(&yellowpen, Rect(100 * i, 50 + 100 * j, 100, 100));
				if (Inputsystem::getInstance().mou_L)
				{
					if ((*mapobject_temp).attr == MapObject_info::SHADOW)
					{
						//LAYER2¿¡ SHADOW°´Ã¼ Ãß°¡
						Map_Layer_2[mappoint.x + i][mappoint.y + j] = (*mapobject_temp);
						Map_Layer_2[mappoint.x + i][mappoint.y + j].attr = (*mapobject_temp).attr;
						Map_Layer_2[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_temp).object_type;
						Map_Layer_2[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
					}
					else if ((*mapobject_temp).object_type == MapObject_info::OBSTACLE)
					{
						obstacle_info *temp = static_cast<obstacle_info *>(mapobject_temp);
						(*temp).pos = { mappoint.x + i,mappoint.y + j };
						GameManager::getInstance().Vobstacle.push_back(*temp);

					}
					else if ((*mapobject_temp).object_type == MapObject_info::ITEM)
					{


					}
					else if ((*mapobject_temp).object_type == MapObject_info::ENEMY)
					{

					}
					else
					{
						if (!clicked)
						{
							preij.x = i;
							preij.y = j;
							premouse = mouse;
						}
						Pen yellowpen(Color(255, 255, 255, 0), 5);
						g.DrawRectangle(&yellowpen, preij.x * 100, 50 + preij.y * 100, (i - preij.x + 1) * 100, (j - preij.y + 1) * 100);
						clicked = true;



					}
				}
				else
				{
					if (clicked)
					{
						int minx, maxx, miny, maxy;
						preij.x > i ? (minx = preij.x, maxx = i) : (maxx = i, minx = preij.x);
						preij.y > j ? (miny = preij.y, maxy = j) : (maxy = j, miny = preij.y);

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

									Map_Layer_1[mappoint.x + i][mappoint.y + j] = (*mapobject_temp);
									Map_Layer_1[mappoint.x + i][mappoint.y + j].attr = (*mapobject_temp).attr;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_temp).object_type;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
							}

						}
						else
						{
							for (int i = minx; i <= maxx; i++)
								for (int j = miny; j <= maxy; j++)
								{
									Map_Layer_1[mappoint.x + i][mappoint.y + j] = (*mapobject_temp);
									Map_Layer_1[mappoint.x + i][mappoint.y + j].attr = (*mapobject_temp).attr;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].object_type = (*mapobject_temp).object_type;
									Map_Layer_1[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
						}


					}
					clicked = false;
				}

				if (Inputsystem::getInstance().mou_R)
					Map_Layer_1[mappoint.x + i][mappoint.y + j].reset();
			}
		}
	}
	//Obstacle
	for (int i = 0; i < GameManager::getInstance().Vobstacle.size(); i++)
	{
		obstacle_info temp = GameManager::getInstance().Vobstacle[i];
		LoadTile(hMemDC, &temp, 100 * (temp.pos.x - mappoint.x), 50 + 100 * (temp.pos.y - mappoint.y));
	}

	//item
	for (int i = 0; i < GameManager::getInstance().Vobstacle.size(); i++)
	{

	}

	//enemy
	for (int i = 0; i < GameManager::getInstance().Vobstacle.size(); i++)
	{

	}

	if (Inputsystem::getInstance().key_W)
	{
		mappoint.y--;
		if (mappoint.y < 0)
			mappoint.y = 0;
	}
	if (Inputsystem::getInstance().key_S)
	{
		mappoint.y++;
		if (mappoint.y > 40)
			mappoint.y = 40;
	}
	if (Inputsystem::getInstance().key_A)
	{
		mappoint.x--;
		if (mappoint.x < 0)
			mappoint.x = 0;
	}
	if (Inputsystem::getInstance().key_D)
	{
		mappoint.x++;
		if (mappoint.x > 40)
			mappoint.x = 40;
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
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					submap[i][j].reset();
				}
			}
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
				(*mapobject_temp).attr = MapObject_info::ROAD;
				break;
			case 9://BLOCK
				(*mapobject_temp).attr = MapObject_info::BLOCK;
				break;
			case 10://SHADOW
				(*mapobject_temp).attr = MapObject_info::SHADOW;
				break;
			case 11://SAVE
			break;
			case 12://LOAD
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
				if ((*mapobject_temp).attr == MapObject_info::ROAD && i == 8)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if((*mapobject_temp).attr == MapObject_info::BLOCK && i == 9)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if((*mapobject_temp).attr == MapObject_info::SHADOW && i == 10)
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
		setweapon();
		break;
	case 6:
		setetc();
		break;
	}

	time_t start, end;
	start = clock();
	Pen blackpen(Color(255, 0, 0, 0), 1);
	for (int i = 0; i < 10; i++)
	{
		g.DrawLine(&blackpen, 1000 + 100 * i, 0, 1000 + 100 * i, 600);
	}

	for (int i = 0; i < 7; i++)
	{
		g.DrawLine(&blackpen, 1000, 100 * i, 1900, 100 * i);
	}

	Pen redpen(Color(255, 255, 0, 0), 5);
	g.DrawRectangle(&redpen, Rect(1000 + 100 * submappoint.x, 100 * submappoint.y, 100, 100));

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (submap[i][j].in_use)
			{
				LoadTile(hdc, &submap[i][j], 1000 + 100 * i, 100 * j);

				Pen yellowpen(Color(255, 255, 0, 255), 3);
				if (mouse.x >= 1000 + 100 * i && mouse.x <= 1100 + 100 * i &&
					mouse.y >= 100 * j && mouse.y <= 100 + 100 * j)
				{
					g.DrawRectangle(&yellowpen, Rect(1000 + 100 * i, 100 * j, 100, 100));
					if (Inputsystem::getInstance().mou_L)
					{
						submappoint.x = i;
						submappoint.y = j;
						(*mapobject_temp).reset();

						if (mapobject_temp)
							delete mapobject_temp;

						if (dynamic_cast<obstacle_info *>(submap[i] + j))
						{
							mapobject_temp = new obstacle_info;
							*mapobject_temp = *dynamic_cast<obstacle_info *>(&submap[i][j]);
						}
						else if (dynamic_cast<item_info *>(submap[i] + j))
						{
							mapobject_temp = new item_info;
							*mapobject_temp = *dynamic_cast<item_info *>(&submap[i][j]);
						}
						else if (dynamic_cast<enemy_info *>(submap[i] + j))
						{
							mapobject_temp = new enemy_info;
							*mapobject_temp = *dynamic_cast<enemy_info *>(&submap[i][j]);
						}
						else 
						{
							mapobject_temp = new MapObject_info;
							*mapobject_temp = *dynamic_cast<MapObject_info *>(&submap[i][j]);
						}

					}
				}
			}
		}
	}
}

void MapManager::MapCursor_temp(HDC hdc)
{
	if ((*mapobject_temp).in_use)
	{
		POINT mouse = GameManager::getInstance().getmouse();
		LoadTile(hdc, mapobject_temp, mouse.x + 50, mouse.y + 50);
	}
}

void MapManager::LoadTile(HDC hdc, MapObject_info *map, int i, int j)
{
	Graphics g(hdc);

	Matrix mat;
	mat.RotateAt((*map).rotation % 360,
		Gdiplus::PointF(i + 50,
			j  + 50));

	g.SetTransform(&mat);

	Rect rect(i, j, (*map).imgrow, (*map).imgcol);
	g.DrawImage(returnimagepointer((*map).col, (*map).row), rect,
		0, 0,
		(*map).imgrow, (*map).imgcol, UnitPixel);








	if ((*map).attr == MapObject_info::BLOCK)
	{
		Pen redpen(Color(255, 255, 0, 0), 5);
		g.DrawRectangle(&redpen, Rect(i, j, 100, 100));
	}

}


void MapManager::setship()
{
	
	//		const int csvrow = 2;
	//		const int csvcol = 0;
	//		const int filenum = 7; 	
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			submap[j][i].in_use = true;
			submap[j][i].col = 2;
			submap[j][i].row = a++;
			submap[j][i].object_type = MapObject_info::TILE;
		}
	}

	for(int i = 4; i < 6; i++)
		for (int j = 5; j < 9; j++)
		{
			if (a == 28) break;
			submap[j][i].in_use = true;
			submap[j][i].col = 2;
			submap[j][i].row = a++;
			submap[j][i].object_type = MapObject_info::TILE;
		}


}

void MapManager::setdesert()
{
	
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[j][i].in_use = true;
			submap[j][i].col = 3;
			submap[j][i].row = a++;
			submap[j][i].object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 3;
		submap[0][i].row = a++;
		submap[0][i].object_type = MapObject_info::TILE;
	}

	for (int i = 0; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			submap[j][i].in_use = true;
			submap[j][i].col = 3;
			submap[j][i].imgcol = 177;
			submap[j][i].row = a++;

			submap[j][i].object_type = MapObject_info::SHADOW;
		}
	}



}

void MapManager::setgreen()
{
	
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[j][i].in_use = true;
			submap[j][i].col = 4;
			submap[j][i].row = a++;
			submap[j][i].object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 4;
		submap[0][i].row = a++;
		submap[0][i].object_type = MapObject_info::TILE;
	}

	for (int i = 0; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			submap[j][i].in_use = true;
			submap[j][i].col = 4;
			submap[j][i].imgcol = 177;
			submap[j][i].row = a++;

			submap[j][i].object_type = MapObject_info::SHADOW;
		}
	}

}

void MapManager::setvolcano()
{
	
	int a = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[j][i].in_use = true;
			submap[j][i].col = 5;
			submap[j][i].row = a++;
			submap[j][i].object_type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 5;
		submap[0][i].row = a++;
		submap[0][i].object_type = MapObject_info::TILE;
	}

	for (int i = 0; i < 6; i += 2)
	{
		for (int j = 4; j < 9; j++)
		{
			if (a == 28) break;
			submap[j][i].in_use = true;
			submap[j][i].col = 5;
			submap[j][i].imgcol = 177;
			submap[j][i].row = a++;

			submap[j][i].object_type = MapObject_info::SHADOW;
		}
	}
}

void MapManager::setobstacle()
{
	int a = 0;
	obstacle_info temp;


	temp.in_use = true;
	temp.col = 6;
	temp.row = a++;
	temp.object_type = MapObject_info::OBSTACLE;
	temp.attr = MapObject_info::OBJECT;
	temp.imgcol = 131;

	temp.obstacle_type = obstacle_info::STATIC;
	temp.kind = obstacle_info::BOX;
	submap[0][0] = *dynamic_cast<obstacle_info *>(&temp);


	temp.row = a++;
	temp.imgcol = 150;
	submap[0][2] = *dynamic_cast<obstacle_info *>(&temp);


	temp.row = a++;
	temp.imgrow = 200;
	temp.imgcol = 150;
	submap[0][4] = *dynamic_cast<obstacle_info *>(&temp);

	temp.row = a++;
	temp.imgrow = 230;
	temp.imgcol = 100;
	temp.kind = obstacle_info::ROOT;
	submap[2][0] = *dynamic_cast<obstacle_info *>(&temp);

	temp.row = a++;
	temp.imgrow = 235;
	temp.imgcol = 100;
	submap[2][2] = *dynamic_cast<obstacle_info *>(&temp);

	temp.row = a++;
	temp.imgrow = 279;
	temp.imgcol = 200;
	submap[5][1] = *dynamic_cast<obstacle_info *>(&temp);

}

void MapManager::setcage()
{

}

void MapManager::setweapon()
{
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
	}
	


	return nullptr;
}

