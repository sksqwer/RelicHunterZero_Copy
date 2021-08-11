#include "GameScreen.h"

#include "GameManager.h"
#include "Mapmanager.h"
#include "GM_Util.h"

void GameScreen::init()
{
	Map_Layer_1 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_1[i] = new MapObject_info[mapsize];

	Map_Layer_2 = new MapObject_info*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_2[i] = new MapObject_info[mapsize];

	MapManager::getInstance().load_map(Map_Layer_1, Map_Layer_2, Vobstacle, Vitem, Venemy, stage1);

	player = new Player;
	player->init();
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

	player->shutdown();
	delete player;
	player = NULL;
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
	print_bullet(hMemDC);
	print_Player_shadow(hMemDC);


	if (Inputsystem::getInstance().mou_R)
		GameManager::getInstance().setScreenflag(1);

	GameManager::getInstance().show_FPS(hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y);

	ScreenManager::getInstance().SetCursor(hMemDC, mapinpoint.x, mapinpoint.y, 2);

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
			MapManager::getInstance().LoadTile(hdc, temp, 100 * ((*temp).pos.x - mapoutpoint.x), -25 + 100 * ((*temp).pos.y - mapoutpoint.y), false);
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


	if (mouse.x - player->mappos.x > 0)
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

		if (mouse.x - player->mappos.x > 0)
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
	//총 구현

	Matrix mat;
	Image *gunimg = GameManager::getInstance().returnimagepointer(player->curgun->col, player->curgun->row);
	int gun_width = gunimg->GetWidth(), gun_height = gunimg->GetHeight();

	

	if (mouse.x > player->mappos.x)
	{
		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });
	}
	else
	{
		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x - 2 * (pos.X - player->mappos.x), mouse.y });
	}
//	rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

	mat.RotateAt(rotation % 360,
		Gdiplus::PointF(pos.X + gun_width / 2,
			pos.Y + 25 + gun_height / 2));
	g.SetTransform(&mat);

	if (mouse.x - player->mappos.x > 0)
	{
		g.DrawImage(gunimg, pos.X, pos.Y + 25);
	}
	else
	{
		g.DrawImage(gunimg,
			pos.X, pos.Y + 25 + gun_height,
			gun_width,
			-1 * gun_height);
	}

}

void GameScreen::print_bullet(HDC hdc)
{
	Gdiplus::Graphics g(hdc);
	Image *img;GameManager::getInstance().returnimagepointer(player->col, player->row);
	for (int i = 0; i < player->bullet_pool.size(); i++)
	{
		if (player->bullet_pool[i]->state == Bullet::SHOOT)
		{
			img = GameManager::getInstance().returnimagepointer(player->bullet_pool[i]->col, player->bullet_pool[i]->row);

			g.DrawImage(img, INT(player->bullet_pool[i]->cur.x - (img->GetWidth() * 0.5)) - (mapoutpoint.x - player->bullet_pool[i]->bulletmapoutpoint.x) * 100 ,
				INT(player->bullet_pool[i]->cur.y - (img->GetHeight() * 0.5) - (mapoutpoint.y - player->bullet_pool[i]->bulletmapoutpoint.y) * 100));
		}
		else
		{
			player->bullet_pool.erase(player->bullet_pool.begin() + i);
			i--;
		}
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


	int width = img->GetWidth();
	int height = img->GetHeight();

	if (mouse.x - player->mappos.x > 0)
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

		if (mouse.x - player->mappos.x > 0)
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
	//bullet
	//총 구현

	Matrix mat;
	Image *gunimg = GameManager::getInstance().returnimagepointer(player->curgun->col, player->curgun->row);
	int gun_width = gunimg->GetWidth(), gun_height = gunimg->GetHeight();



	if (mouse.x > player->mappos.x)
	{
		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });
	}
	else
	{
		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x - 2 * (pos.X - player->mappos.x), mouse.y });
	}
	//	rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

	mat.RotateAt((360 - rotation) % 360,
		Gdiplus::PointF(pos.X + gun_width / 2, pos.Y + 25 + gun_height));
	g.SetTransform(&mat);

	if (mouse.x - player->mappos.x < 0)
	{
		g.DrawImage(gunimg, Rect(pos.X, pos.Y - 25 +  gun_height, gun_width,  gun_height),
			0, 0, gun_width, gun_height,
			UnitPixel, &imageAtt);
	}
	else
	{
		g.DrawImage(gunimg, Rect(pos.X, pos.Y - 25 + 2 * gun_height, gun_width, -1 * gun_height),
			0, 0, gun_width, gun_height,
			UnitPixel, &imageAtt);
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
	premapinpoint = mapinpoint;
	premapoutpoint = mapoutpoint;
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
			if (mapoutpoint.y > 84)
			{
				mapoutpoint.y = 84;
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
			if (mapoutpoint.x > 76)
			{
				mapoutpoint.x = 76;
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

	collide_player();
	//bullet
	update_bullet();

	mouse = GameManager::getInstance().getmouse();
	if (Inputsystem::getInstance().mou_L)
	{
		DWORD newTime = GetTickCount();
		static DWORD shooting_oldTime = 0;
		if (newTime - shooting_oldTime >= player->curgun->rate)
		{
			shooting_oldTime = newTime;

			RECT rt;
			GetClientRect(g_hWnd, &rt);

			POINTF dir = { float((mouse.x - 960) * player->curgun->bullet_speed) / float(get_distance(player->mappos,mouse)),
						float((mouse.y - 590) * player->curgun->bullet_speed) / float(get_distance(player->mappos,mouse)) };
			// dir 계산
			POINT cur = { mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y };

			player->shot_bullet(dir, cur, mapoutpoint);
		}
	}

	player->update();
}

void GameScreen::update_bullet()
{
	for (int i = 0; i < player->bullet_using.size(); i++)
	{
//		if (get_distance(player->bullet_using[i]->cur, player->bullet_using[i]->pre) >= player->curgun->range ||
//			Map_Layer_1[mapoutpoint.x + ((mapinpoint.x + player->bullet_using[i]->cur.x) % 100)][mapoutpoint.y + ((mapinpoint.y + player->bullet_using[i]->cur.y) % 100)].on_obj == 3 ||
//			Map_Layer_1[mapoutpoint.x + ((mapinpoint.x + player->bullet_using[i]->cur.x) % 100)][mapoutpoint.y + ((mapinpoint.y + player->bullet_using[i]->cur.y) % 100)].attr == MapObject_info::BLOCK )
//		{
//			player->bullet_using[i]->state = Bullet::DESTROY;
//			continue;
//		}

		player->bullet_using[i]->cur.x += player->bullet_using[i]->dir.x;
		player->bullet_using[i]->cur.y += player->bullet_using[i]->dir.y;
	}
}

void GameScreen::collide_player()
{
	// player의 중심mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y
	POINT pointinterval;
	if (mapoutpoint.x * 100 + mapinpoint.x > premapoutpoint.x * 100 + premapinpoint.x)
		pointinterval.x = -2;
	else if (mapoutpoint.x * 100 + mapinpoint.x < premapoutpoint.x * 100 + premapinpoint.x)
		pointinterval.x = 2;
	else
		pointinterval.x = 0;

	if (mapoutpoint.y * 100 + mapinpoint.y > premapoutpoint.y * 100 + premapinpoint.y)
		pointinterval.y = -2;
	else if (mapoutpoint.y * 100 + mapinpoint.y < premapoutpoint.y * 100 + premapinpoint.y)
		pointinterval.y = 2;
	else
		pointinterval.y = 0;

	while (Map_Layer_1[(mapoutpoint.x * 100 + mapinpoint.x + player->mappos.x - 17) / 100][(mapoutpoint.y * 100 + mapinpoint.y + player->mappos.y + 47) / 100].attr == MapObject_info::BLOCK  ||
		Map_Layer_1[(mapoutpoint.x * 100 + mapinpoint.x + player->mappos.x + 17) / 100][(mapoutpoint.y * 100 + mapinpoint.y + player->mappos.y + 47) / 100].attr == MapObject_info::BLOCK )
	{
		mapinpoint.x += pointinterval.x;
		mapinpoint.y += pointinterval.y;
		if (mapinpoint.x < 100)
		{
			mapoutpoint.x--;
			mapinpoint.x += 100;
			if (mapoutpoint.x < 0)
			{
				mapinpoint.x -= 100;
				mapoutpoint.x = 0;
			}
		}
		else if (mapinpoint.x > 300)
		{
			mapoutpoint.x++;
			mapinpoint.x -= 100;
			if (mapoutpoint.x > 76)
			{
				mapinpoint.x += 100;
				mapoutpoint.x = 76;
			}
		}
		if (mapinpoint.y < 100)
		{
			mapoutpoint.y--;
			mapinpoint.y += 100;
			if (mapoutpoint.y < 0)
			{
				mapinpoint.y -= 100;
				mapoutpoint.y = 0;
			}
				
		}
		else if (mapinpoint.y > 300)
		{
			mapoutpoint.y++;
			mapinpoint.y -= 100;
			if (mapoutpoint.y > 84)
			{
				mapinpoint.y += 100;
				mapoutpoint.y = 84;
			}
		}
	}


}
