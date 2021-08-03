#include "MapManager.h"
#include "GameManager.h"

void MapManager::Edit_init()
{
	mapinfo = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		mapinfo[i] = new MapObject_info[mapsize];



}

void MapManager::Edit_shutdown()
{
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

void MapManager::TotalView(HWND hWnd, HDC hdc)
{
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x, by = size.y;

//	int bx = 1920, by = 1180;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);


	const int csvrow = 0;
	const int csvcol = 0;
	const int filenum = 1;

	char path[1][100];
	TCHAR Tpath[1][100];

	Filesystem::getInstance().getpath(csvrow, csvcol, path, filenum);


	int msglen = MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), Tpath[0], msglen);
	Tpath[0][msglen] = NULL;

	Image bgimg(Tpath[0]);

	Graphics g(hMemDC);
	g.DrawImage(&bgimg, 0, 0);

	Pen      pen(Color(255, 0, 255, 0), 5);
	g.DrawLine(&pen, 1000, 0, 1000, 1180);

	MainView(hWnd, hMemDC);

	SubView(hWnd, hMemDC);

	ScreenManager::getInstance().SetCursor(hWnd, hMemDC, 0, 0, 0);
	MapCursor_temp(hWnd, hMemDC);

	BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);

	if (Inputsystem::getInstance().key_Q)
		mapobject_temp.rotation -= 90;

	if (Inputsystem::getInstance().key_E)
		mapobject_temp.rotation += 90;

	mapobject_temp.rotation %= 360;

}

void MapManager::MainView(HWND hWnd, HDC hdc)
{
	POINT size = GameManager::getInstance().getsize();
	int bx = 1000, by = size.y;
	POINT mouse = GameManager::getInstance().getmouse();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);

	Graphics g(hMemDC);


	Pen pen(Color(255, 0, 0, 0), 1);
	SolidBrush  brush(Color(255, 255, 255, 255));
	g.FillRectangle(&brush, 0, 50, 1000, 1000);
	for (int i = 0; i < 10; i++)
	{
			g.DrawLine(&pen, 100 * i, 50, 100 * i, 1050);
			g.DrawLine(&pen, 0, 100 * i + 50, 1000, 100 * i + 50);
	}


	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapinfo[mappoint.x + i][mappoint.y + j].in_use)
			{
				LoadTile(hMemDC, mapinfo[mappoint.x + i][mappoint.y + j], 100 * i, 50 + 100 * j);
				
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

void MapManager::SubView(HWND hWnd, HDC hdc)
{
	MapObjectView(hWnd, hdc);

	int msglen;
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x - 1000, by = size.y;

	const int csvrow = 1;
	const int csvcol = 0;
	const int filenum = 3;

	char path[filenum][100];
	TCHAR Tpath[filenum][100];
	Image *img[filenum];

	Filesystem::getInstance().getpath(csvrow, csvcol, path, filenum);

	for (int i = 0; i < filenum; i++)
	{
		msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		img[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}

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
			g.DrawImage(img[1], coor[i][0], coor[i][1]);

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
					g.DrawImage(img[2], coor[i][0], coor[i][1]);
				}
				else if(mapobject_temp.attr == MapObject_info::BLOCK && i == 9)
				{
					g.DrawImage(img[2], coor[i][0], coor[i][1]);
				}
				else if(mapobject_temp.attr == MapObject_info::SHADOW && i == 10)
				{
					g.DrawImage(img[2], coor[i][0], coor[i][1]);
				}
				else
				{
					g.DrawImage(img[0], coor[i][0], coor[i][1]);
				}

			}
			else
				g.DrawImage(img[0], coor[i][0], coor[i][1]);
		}
		ScreenManager::getInstance().DoubleBufferingtext(hWnd, hdc, tstr[i], coor[i][0] + 30, coor[i][1] + 5, 100, 40, 20, Color(255, 0, 0, 0));
	}












	for (int i = 0; i < filenum; i++)
	{
		delete img[i];
	}
}

void MapManager::MapObjectView(HWND hWnd, HDC hdc)
{
	Graphics g(hdc);

	SolidBrush  brush(Color(255, 200, 200, 200));
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
						submappoint.x = i;
						submappoint.y = j;
						mapobject_temp = submap[i][j];
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

void MapManager::MapCursor_temp(HWND hWnd, HDC hdc)
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

	const int filenum = 1;
	char path[filenum][100];
	TCHAR Tpath[filenum][100];
	Filesystem::getInstance().getpath(map.CSVrow, map.CSVcol, path, filenum);
	int msglen = MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), Tpath[0], msglen);
	Tpath[0][msglen] = NULL;

	Image img(Tpath[0]);

	Matrix mat;
	mat.RotateAt(map.rotation % 360,
		Gdiplus::PointF(i + 50,
			j  + 50));

	g.SetTransform(&mat);

	g.DrawImage(&img, Rect(i, j, 100, 100),
		map.imgrow, map.imgcol,
		100, 100, UnitPixel);

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

void MapManager::setship()
{
	//		const int csvrow = 2;
	//		const int csvcol = 0;
	//		const int filenum = 7; 	

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[i][j].in_use = true;
			submap[i][j].CSVrow = 2;
			submap[i][j].CSVcol = 0;
			submap[i][j].imgrow = i * 100;
			submap[i][j].imgcol = j * 100;
			submap[i][j].type = MapObject_info::TILE;
		}
	}
	for (int i = 5; i < 9; i++)
	{
		submap[i][0].in_use = true;
		submap[i][0].CSVrow = 2;
		submap[i][0].CSVcol = i - 4;
		submap[i][0].type = MapObject_info::TILE;
	}
	for (int i = 5; i < 7; i++)
	{
		submap[i][1].in_use = true;
		submap[i][1].CSVrow = 2;
		submap[i][1].CSVcol = i;
		submap[i][1].type = MapObject_info::TILE;
	}
}

void MapManager::setdesert()
{
	//		const int csvrow = 3;
	//		const int csvcol = 0;
	//		const int filenum = 12; 	

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[i][j].in_use = true;
			submap[i][j].CSVrow = 3;
			submap[i][j].CSVcol = 0;
			submap[i][j].imgrow = i * 100;
			submap[i][j].imgcol = j * 100;
			submap[i][j].type = MapObject_info::TILE;
			submap[7][i].attr = MapObject_info::BLOCK;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].CSVrow = 3;
		submap[0][i].CSVcol = 1;
		submap[0][i].imgrow = 0;
		submap[0][i].imgcol = (i - 4) * 100;
		submap[0][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::ROAD;
	}
	for (int i = 0; i < 2; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 3;
		submap[4][i].CSVcol = 2;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = i * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 3;
		submap[4][i].CSVcol = 3;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 2) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 3;
		submap[4][i].CSVcol = 4;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 4) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 3;
		submap[5][i].CSVcol = 5;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = i * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 3;
		submap[5][i].CSVcol = 6;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 2) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 3;
		submap[5][i].CSVcol = 7;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 4) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 3;
		submap[6][i].CSVcol = 8;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = i * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 3;
		submap[6][i].CSVcol = 9;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 2) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 3;
		submap[6][i].CSVcol = 10;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 4) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 3;
		submap[7][i].CSVcol = 11;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = i * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 3;
		submap[7][i].CSVcol = 12;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = (i - 2) * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
}

void MapManager::setgreen()
{
	//		const int csvrow = 4;
	//		const int csvcol = 0;
	//		const int filenum = 12; 	

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[i][j].in_use = true;
			submap[i][j].CSVrow = 4;
			submap[i][j].CSVcol = 0;
			submap[i][j].imgrow = i * 100;
			submap[i][j].imgcol = j * 100;
			submap[i][j].type = MapObject_info::TILE;
			submap[7][i].attr = MapObject_info::BLOCK;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].CSVrow = 4;
		submap[0][i].CSVcol = 1;
		submap[0][i].imgrow = 0;
		submap[0][i].imgcol = (i - 4) * 100;
		submap[0][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::ROAD;
	}
	for (int i = 0; i < 2; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 4;
		submap[4][i].CSVcol = 2;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = i * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 4;
		submap[4][i].CSVcol = 3;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 2) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 4;
		submap[4][i].CSVcol = 4;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 4) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 4;
		submap[5][i].CSVcol = 5;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = i * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 4;
		submap[5][i].CSVcol = 6;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 2) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 4;
		submap[5][i].CSVcol = 7;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 4) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 4;
		submap[6][i].CSVcol = 8;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = i * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 4;
		submap[6][i].CSVcol = 9;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 2) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 4;
		submap[6][i].CSVcol = 10;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 4) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 4;
		submap[7][i].CSVcol = 11;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = i * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 4;
		submap[7][i].CSVcol = 12;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = (i - 2) * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
}

void MapManager::setvolcano()
{
	//		const int csvrow = 5;
	//		const int csvcol = 0;
	//		const int filenum = 12; 	

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			submap[i][j].in_use = true;
			submap[i][j].CSVrow = 5;
			submap[i][j].CSVcol = 0;
			submap[i][j].imgrow = i * 100;
			submap[i][j].imgcol = j * 100;
			submap[i][j].type = MapObject_info::TILE;
			submap[7][i].attr = MapObject_info::BLOCK;
		}
	}
	for (int i = 4; i < 6; i++)
	{
		submap[0][i].in_use = true;
		submap[0][i].CSVrow = 5;
		submap[0][i].CSVcol = 1;
		submap[0][i].imgrow = 0;
		submap[0][i].imgcol = (i - 4) * 100;
		submap[0][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::ROAD;
	}
	for (int i = 0; i < 2; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 5;
		submap[4][i].CSVcol = 2;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = i * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 5;
		submap[4][i].CSVcol = 3;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 2) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[4][i].in_use = true;
		submap[4][i].CSVrow = 5;
		submap[4][i].CSVcol = 4;
		submap[4][i].imgrow = 0;
		submap[4][i].imgcol = (i - 4) * 100;
		submap[4][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 5;
		submap[5][i].CSVcol = 5;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = i * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 5;
		submap[5][i].CSVcol = 6;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 2) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[5][i].in_use = true;
		submap[5][i].CSVrow = 5;
		submap[5][i].CSVcol = 7;
		submap[5][i].imgrow = 0;
		submap[5][i].imgcol = (i - 4) * 100;
		submap[5][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 5;
		submap[6][i].CSVcol = 8;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = i * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 5;
		submap[6][i].CSVcol = 9;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 2) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 4; i < 6; i++)
	{
		submap[6][i].in_use = true;
		submap[6][i].CSVrow = 5;
		submap[6][i].CSVcol = 10;
		submap[6][i].imgrow = 0;
		submap[6][i].imgcol = (i - 4) * 100;
		submap[6][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	//
	for (int i = 0; i < 2; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 5;
		submap[7][i].CSVcol = 11;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = i * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
	for (int i = 2; i < 4; i++)
	{
		submap[7][i].in_use = true;
		submap[7][i].CSVrow = 5;
		submap[7][i].CSVcol = 12;
		submap[7][i].imgrow = 0;
		submap[7][i].imgcol = (i - 2) * 100;
		submap[7][i].type = MapObject_info::TILE;
		submap[7][i].attr = MapObject_info::SHADOW;
	}
}

