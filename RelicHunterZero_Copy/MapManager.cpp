#include "MapManager.h"
#include "GameManager.h"
#include <time.h>

void MapManager::Edit_init()
{
	mapinfo = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		mapinfo[i] = new MapObject_info[mapsize];


	void LoadImage();

}

void MapManager::Edit_shutdown()
{
	Vobstacle.clear();
	Vitem.clear();
	Venemy.clear();

	for(int i = 0 ; i < 50; i++)
		for (int j = 0; j < 50; j++)
		{
			if (mapinfo[i][j].Shadow)
				delete mapinfo[i][j].Shadow;
		}

	for (int i = 0; i < mapsize; i++)
		delete mapinfo[i];

	delete mapinfo;


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
		mapobject_temp.rotation -= 90;

	if (Inputsystem::getInstance().key_E)
		mapobject_temp.rotation += 90;

	mapobject_temp.rotation %= 360;

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
			if (mapinfo[mappoint.x + i][mappoint.y + j].in_use)
			{
				LoadTile(hMemDC, mapinfo[mappoint.x + i][mappoint.y + j], 100 * i, 50 + 100 * j);
				
				for (int i = 0; i < Vobstacle.size(); i++)
					LoadTile(hMemDC, Vobstacle[i]);

				for (int i = 0; i < Vitem.size(); i++)
					LoadTile(hMemDC, Vitem[i]);

				for (int i = 0; i < Venemy.size(); i++)
					LoadTile(hMemDC, Venemy[i]);
			}
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
					if (mapobject_temp.attr == MapObject_info::SHADOW)
					{
						if (mapinfo[mappoint.x + i][mappoint.y + j].Shadow)
							delete mapinfo[mappoint.x + i][mappoint.y + j].Shadow;

						mapinfo[mappoint.x + i][mappoint.y + j].Shadow = new MapObject_info;
						*mapinfo[mappoint.x + i][mappoint.y + j].Shadow = mapobject_temp;
						(*mapinfo[mappoint.x + i][mappoint.y + j].Shadow).attr = mapobject_temp.attr;
						(*mapinfo[mappoint.x + i][mappoint.y + j].Shadow).type = mapobject_temp.type;
						(*mapinfo[mappoint.x + i][mappoint.y + j].Shadow).pos = { mappoint.x + i,mappoint.y + j };
					}
					else if (mapobject_temp.type == MapObject_info::OBSTACLE)
					{
						mapinfo[mappoint.x + i][mappoint.y + j].in_use = true;
						mapobject_temp.obstacle_temp.pos = { mappoint.x + i,mappoint.y + j };
						Vobstacle.push_back(mapobject_temp.obstacle_temp);

					}
					else if (mapobject_temp.type == MapObject_info::ITEM)
					{
						mapinfo[mappoint.x + i][mappoint.y + j].in_use = true;
						mapobject_temp.item_temp.pos = { mappoint.x + i,mappoint.y + j };
						Vitem.push_back(mapobject_temp.item_temp);

					}
					else if (mapobject_temp.type == MapObject_info::ENEMY)
					{
						mapinfo[mappoint.x + i][mappoint.y + j].in_use = true;
						mapobject_temp.enemy_temp.pos = { mappoint.x + i,mappoint.y + j };
						Venemy.push_back(mapobject_temp.enemy_temp);
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

							static bool skip = true;
						
							for (int i = minx; i <= maxx; i++)
							{
								if (!((maxx - minx + 1) % 2))
								{
									if (skip)
									{
										skip = false;
									}
									else
									{
										skip = true;

									}
								}
								for (int j = miny; j <= maxy; j++)
								{

									if (skip)
									{
										skip = false;
										continue;
									}
									else
									{
										skip = true;

									}


									mapinfo[mappoint.x + i][mappoint.y + j] = mapobject_temp;
									mapinfo[mappoint.x + i][mappoint.y + j].attr = mapobject_temp.attr;
									mapinfo[mappoint.x + i][mappoint.y + j].type = mapobject_temp.type;
									mapinfo[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
							}
								
						}
						else
						{
							for (int i = minx; i <= maxx; i++)
								for (int j = miny; j <= maxy; j++)
								{
									mapinfo[mappoint.x + i][mappoint.y + j] = mapobject_temp;
									mapinfo[mappoint.x + i][mappoint.y + j].attr = mapobject_temp.attr;
									mapinfo[mappoint.x + i][mappoint.y + j].type = mapobject_temp.type;
									mapinfo[mappoint.x + i][mappoint.y + j].pos = { mappoint.x + i,mappoint.y + j };
								}
						}


					}
					clicked = false;
				}

				if (Inputsystem::getInstance().mou_R)
					mapinfo[mappoint.x + i][mappoint.y + j].MapObject_reset();
			}

		}
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

	int coor[button_num][2] = { {1050, 700}, {1250, 700}, {1450, 700}, {1650, 700}, {1050, 800}, {1250, 800}, {1450, 800}, {1650, 800},
						{1450, 900}, {1450, 1000},{1450, 1100}, {1650, 900}, {1650, 1000}, {1650, 1100} };

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
				mapobject_temp.attr = MapObject_info::ROAD;
				break;
			case 9://BLOCK
				mapobject_temp.attr = MapObject_info::BLOCK;
				break;
			case 10://SHADOW
				mapobject_temp.attr = MapObject_info::SHADOW;
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
			if (i == 9 || i == 10 || i == 8)
			{
				if (mapobject_temp.attr == MapObject_info::ROAD && i == 8)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if(mapobject_temp.attr == MapObject_info::BLOCK && i == 9)
				{
					g.DrawImage((GameManager::getInstance().maptoggle)[2], coor[i][0], coor[i][1]);
				}
				else if(mapobject_temp.attr == MapObject_info::SHADOW && i == 10)
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
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			submap[i][j].MapObject_reset();
		}
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
		setweapon();
	break;
	case 6:
		setetc();
	break;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (submap[i][j].in_use)
			{
				LoadTile(hdc, submap[i][j], 1000 + 100 * i, 100 * j);

				Pen yellowpen(Color(255, 255, 0, 255), 3);
				if (mouse.x >= 1000 + 100 * i && mouse.x <= 1100 + 100 * i &&
					mouse.y >= 100 * j && mouse.y <= 100 + 100 * j)
				{
					g.DrawRectangle(&yellowpen, Rect(1000 + 100 * i, 100 * j, 100, 100));
					if (Inputsystem::getInstance().mou_L)
					{
						mapobject_temp.MapObject_reset();
						submappoint.x = i;
						submappoint.y = j;
						mapobject_temp = submap[i][j];
						mapobject_temp.obstacle_temp = submap[i][j].obstacle_temp;
						mapobject_temp.item_temp = submap[i][j].item_temp;
						mapobject_temp.enemy_temp = submap[i][j].enemy_temp;
						mapobject_temp.type = submap[i][j].type;
					}
				}
			}
		}
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


	Pen redpen(Color(255, 255, 0, 0), 5);
	g.DrawRectangle(&redpen, Rect(1000 + 100 * submappoint.x, 100 * submappoint.y, 100, 100));




}

void MapManager::MapCursor_temp(HDC hdc)
{
	if (mapobject_temp.in_use)
	{
		POINT mouse = GameManager::getInstance().getmouse();
		LoadTile(hdc, mapobject_temp, mouse.x + 50, mouse.y + 50);
	}
}

void MapManager::LoadTile(HDC hdc, MapObject_info map, int i, int j)
{
	Graphics g(hdc);

	Matrix mat;
	mat.RotateAt(map.rotation % 360,
		Gdiplus::PointF(i + 50,
			j  + 50));

	g.SetTransform(&mat);

	Rect rect(i, j, map.imgrow, map.imgcol);
	g.DrawImage(returnimagepointer(map.col, map.row), rect,
		0, 0,
		map.imgrow, map.imgcol, UnitPixel);


	//Rect rect(i, j - (map.imgsizecol - 100), 100 * map.row, 100 * map.col);
	//g.DrawImage(&img, rect,
	//	map.imgcutrow, map.imgcutcol,
	//	100 * map.row, 100 * map.col, UnitPixel);









	if (map.Shadow)
	{
		LoadTile(hdc, *(map.Shadow), i, j);
	}

	if (map.attr == MapObject_info::BLOCK)
	{
		Pen redpen(Color(255, 255, 0, 0), 5);
		g.DrawRectangle(&redpen, Rect(i, j, 100, 100));
	}

}

void MapManager::LoadTile(HDC hdc, obstacle_info &o)
{
	Graphics g(hdc);

	const int filenum = 1;
	char path[filenum][100];
	TCHAR Tpath[filenum][100];
	Filesystem::getInstance().getpath(o.CSVrow, o.CSVcol, path, filenum);
	int msglen = MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), Tpath[0], msglen);
	Tpath[0][msglen] = NULL;

	Image img(Tpath[0]);

	//Matrix mat;
	//mat.RotateAt(o.rotation % 360,
	//	Gdiplus::PointF(i + 50,
	//		j + 50));

	//g.SetTransform(&mat);

	//g.DrawImage(&img, Rect((o.pos.x - mappoint.x) * 100, (o.pos.y - mappoint.y) * 100 - (o.imgsizecol - 100),
	//	o.imgsizerow, o.imgsizecol),
	//	0, 0,
	//	o.imgsizerow, o.imgsizecol, UnitPixel);

	g.DrawImage(&img, Rect((o.pos.x - mappoint.x) * 100, (o.pos.y - mappoint.y) * 100 - (img.GetHeight() - 100) + 50,
		img.GetWidth(), img.GetHeight()),
		0, 0,
		img.GetWidth(), img.GetHeight(), UnitPixel);
}

void MapManager::LoadTile(HDC hdc, item_info &i)
{

}

void MapManager::LoadTile(HDC hdc, enemy_info &e)
{

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
			submap[j][i].type = MapObject_info::TILE;
		}
	}

	for(int i = 4; i < 6; i++)
		for (int j = 5; j < 9; j++)
		{
			if (a == 28) break;
			submap[j][i].in_use = true;
			submap[j][i].col = 2;
			submap[j][i].row = a++;
			submap[j][i].type = MapObject_info::TILE;
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
			submap[j][i].type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 3;
		submap[0][i].row = a++;
		submap[0][i].type = MapObject_info::TILE;
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

			submap[j][i].type = MapObject_info::SHADOW;
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
			submap[j][i].type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 4;
		submap[0][i].row = a++;
		submap[0][i].type = MapObject_info::TILE;
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

			submap[j][i].type = MapObject_info::SHADOW;
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
			submap[j][i].type = MapObject_info::TILE;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].col = 5;
		submap[0][i].row = a++;
		submap[0][i].type = MapObject_info::TILE;
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

			submap[j][i].type = MapObject_info::SHADOW;
		}
	}
}

void MapManager::setobstacle()
{
	//		const int csvrow = 6;
	//		const int csvcol = 0;
	//		const int filenum = 6; 	
	int a = 0;
	for(int i =0; i < 6; i+=2)
		{
			submap[0][i].in_use = true;
			submap[0][i].row = 2;
			submap[0][i].obstacle_temp.type = 0;
			submap[0][i].obstacle_temp.type = 10;
			submap[0][i].obstacle_temp.CSVrow = 6;
			submap[0][i].obstacle_temp.CSVcol = a;
			submap[0][i].obstacle_temp.imgsizecol = 150;
			submap[0][i].type = 1;
		}
	submap[0][4].obstacle_temp.imgsizerow = 200;

	for (int i = 0; i < 6; i+=2)
	{
		submap[2][i].in_use = true;
		submap[2][i].row = 3;
		submap[2][i].obstacle_temp.type = 0;
		submap[2][i].obstacle_temp.type = 11;
		submap[2][i].obstacle_temp.CSVrow = 6;
		submap[2][i].obstacle_temp.CSVcol = a;
		submap[2][i].type = 1;
	}
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

