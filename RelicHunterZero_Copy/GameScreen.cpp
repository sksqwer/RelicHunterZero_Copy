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
	set_player();
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

	delete player;
	player = NULL;
}

void GameScreen::set_player()
{
	player = new Player;


}

void GameScreen::TotalGameview()
{
	HDC hdc = GetDC(g_hWnd);
	RECT rect;
	//	GetWindowRect(g_hWnd, &rect);
	POINT size = GameManager::getInstance().getsize();
	//	POINT size = {rect.right, rect.bottom};
	int bx = size.x + 400, by = size.y + 400;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);

	Gdiplus::Graphics g(hMemDC);



	stage1MapView(hMemDC);



	print_Player(hMemDC); 
	print_Player_shadow(hMemDC);


	if (Inputsystem::getInstance().mou_L)
		GameManager::getInstance().setScreenflag(1);

	GameManager::getInstance().show_FPS(hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y);

	ScreenManager::getInstance().SetCursor(hMemDC, mapinpoint.x, mapinpoint.y, 0);

	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y, SRCCOPY);
	//	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x, mapinpoint.y, SRCCOPY);
	//	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);


	ReleaseDC(g_hWnd, hdc);
}

void GameScreen::stage1MapView(HDC hdc)
{
	for (int i = mapoutpoint.x; i < mapoutpoint.x + 23; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 15; j++)
		{
			if (Map_Layer_1[i][j].in_use)
			{
				MapManager::getInstance().LoadTile(hdc, &Map_Layer_1[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

			}
		}

	for (int i = mapoutpoint.x; i < mapoutpoint.x + 23; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 15; j++)
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

void GameScreen::print_Player(HDC hdc)
{
	Gdiplus::Graphics g(hdc);

	Image *img = GameManager::getInstance().returnimagepointer(player->col, player->row);
	POINT mouse = GameManager::getInstance().getmouse();

	Point pos = { mapinpoint.x + player->mappos.x - 54, mapinpoint.y + player->mappos.y -47 };
	mouse.x -= pos.X;
	mouse.y -= pos.Y;

	if (mouse.x > 0)
	{
		g.DrawImage(img, pos.X, pos.Y);
	}
	else
	{
		g.DrawImage(img, pos.X + img->GetWidth(), pos.Y, -1 * img->GetWidth(), img->GetHeight());
	}

	//state에 따른 이펙트
	switch (player->state)
	{
	case Player::DASH:
	{
		static bool dash_first = true;
		static Point dash_start;
		static int dash_effect = 0;
		if (dash_first)
		{
			dash_start = { mapinpoint.x + player->mappos.x - 84, mapinpoint.y + player->mappos.y - 90 };
			dash_first = false;
		}
		else if (dash_effect == 5)
		{
			dash_effect = 0;
			dash_first = true;
		}

		if (mouse.x > 0)
		{
			load_Image(hdc, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, false);
			load_Image(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 74, mapinpoint.y + player->mappos.y - 47, false);
			load_Image(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 94, mapinpoint.y + player->mappos.y - 47, false);
		}
		else
		{
			load_Image(hdc, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, true);
			load_Image(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 20, mapinpoint.y + player->mappos.y - 47, true);
			load_Image(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y - 47, true);
		}
	}
	break;
	}


}

void GameScreen::print_Player_shadow(HDC hdc)
{
	Gdiplus::Graphics g(hdc);

	Image *img = GameManager::getInstance().returnimagepointer(player->col, player->row);
	POINT mouse = GameManager::getInstance().getmouse();

	REAL rTransparency = 0.5f;

	ColorMatrix colorMatrix =
	{
		0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
		0.588f, 0.588f, 0.588f, 0.0f, 0.0f,
		0.111f, 0.111f, 0.111f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rTransparency, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	Point pos = { mapinpoint.x + player->mappos.x - 54, mapinpoint.y + player->mappos.y - 47 };
	mouse.x -= pos.X;
	mouse.y -= pos.Y;

	int width = img->GetWidth();
	int height = img->GetHeight();

	if (mouse.x > 0)
	{
		g.DrawImage(img, Rect(pos.X, pos.Y + 2 * height, width,  -1 * height),
			0, 0, width, height,
		UnitPixel, &imageAtt);
	}
	else
	{
		g.DrawImage(img, Rect(pos.X + width, pos.Y + 2 * height, -1 * width, -1 * height),
			0, 0, width, height,
			UnitPixel, &imageAtt);
	}

	//state에 따른 이펙트
	switch (player->state)
	{
	case Player::DASH:
	{
		static bool dash_first = true;
		static Point dash_start;
		static int dash_effect = 0;
		if (dash_first)
		{
			dash_start = { mapinpoint.x + player->mappos.x - 84, mapinpoint.y + player->mappos.y - 90 };
			dash_first = false;
		}
		else if (dash_effect == 5)
		{
			dash_effect = 0;
			dash_first = true;
		}

		if (mouse.x > 0)
		{
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, false);
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 74, mapinpoint.y + player->mappos.y - 47, false);
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 94, mapinpoint.y + player->mappos.y - 47, false);
		}
		else
		{
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, true);
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 20, mapinpoint.y + player->mappos.y - 47, true);
			load_Image_shadow(hdc, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y - 47, true);
		}
	}
	break;
	}


}

void GameScreen::load_Image(HDC hdc, Image *img, int a, int b, bool reverse)
{
	Gdiplus::Graphics g(hdc);
	

	if (reverse)
	{
		g.DrawImage(img, a + img->GetWidth(), b, -1 * img->GetWidth(), img->GetHeight());

	}
	else
	{
		g.DrawImage(img, a, b);

	}
}

void GameScreen::load_Image_shadow(HDC hdc, Image * img, int a, int b, bool reverse)
{
	Gdiplus::Graphics g(hdc);
	REAL rTransparency = 0.5f;

	ColorMatrix colorMatrix =
	{
		0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
		0.588f, 0.588f, 0.588f, 0.0f, 0.0f,
		0.111f, 0.111f, 0.111f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rTransparency, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	int width = img->GetWidth();
	int height = img->GetHeight();
	if (reverse)
	{
		g.DrawImage(img, Rect(a + width, b + 2 * height, -1 * width, -1 * height),
			0, 0, width, height,
			UnitPixel, &imageAtt);

	}
	else
	{
		g.DrawImage(img, Rect(a, b + 2 * height, width, -1 * height),
			0, 0, width, height,
			UnitPixel, &imageAtt);

	}
}

void GameScreen::update()
{
	int velocity = player->velocity;
	if (Inputsystem::getInstance().key_W)
	{
		mapinpoint.y -= velocity;
		if (mapinpoint.y <= 100)
		{
			mapoutpoint.y--;
			if (mapoutpoint.y < 0)
			{
				mapoutpoint.y = 0;
			}
			else
				mapinpoint.y += 100;
		}
	}
	else if (Inputsystem::getInstance().key_S)
	{
		mapinpoint.y += velocity;
		if (mapinpoint.y >= 300)
		{
			mapoutpoint.y++;
			if (mapoutpoint.y > 15)
			{
				mapoutpoint.y = 15;
			}
			else
				mapinpoint.y -= 100;


		}

	}

	if (Inputsystem::getInstance().key_A)
	{
		mapinpoint.x -= velocity;
		if (mapinpoint.x <= 100)
		{
			mapoutpoint.x--;
			if (mapoutpoint.x < 0)
			{
				mapoutpoint.x = 0;
			}
			else
				mapinpoint.x += 100;


		}

	}
	else if (Inputsystem::getInstance().key_D)
	{
		mapinpoint.x += velocity;
		if (mapinpoint.x >= 300)
		{
			mapoutpoint.x++;
			if (mapoutpoint.x > 26)
			{
				mapoutpoint.x = 26;
			}
			else
				mapinpoint.x -= 100;

		}

	}

	// 마우스 뷰 조절

	POINT mouse = GameManager::getInstance().getmouse();

	mouse.x -= 960;
	mouse.y -= 590;

	if (mouse.x > 0 && mouse.y <= 0) // + -
	{
		mouseinpoint.x = abs(mouse.x * 100 / 960);
		mouseinpoint.y = abs(mouse.y * 100 / 590) * -1;
	}
	else if (mouse.x <= 0 && mouse.y < 0) // - -
	{
		mouseinpoint.x = abs(mouse.x * 100 / 960) * -1;
		mouseinpoint.y = abs(mouse.y * 100 / 590) * -1;
	}
	else if (mouse.x < 0 && mouse.y >= 0) // - +
	{
		mouseinpoint.x = abs(mouse.x * 100 / 960) * -1;
		mouseinpoint.y = abs(mouse.y * 100 / 590);
	}
	else // + +
	{
		mouseinpoint.x = abs(mouse.x * 100 / 960);
		mouseinpoint.y = abs(mouse.y * 100 / 590);
	}

	player->update();
}
