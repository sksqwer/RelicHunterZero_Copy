#include "GameScreen.h"

#include "GameManager.h"
#include "Mapmanager.h"
#include "GM_Util.h"

void GameScreen::init()
{
	mapImage = new HBITMAP;
	*mapImage = (HBITMAP)LoadImage(NULL, L"Resource/Save/stage1.png",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	mapImagepng = Gdiplus::Image::FromFile(L"Resource/Save/stage12.png");

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
	DeleteObject(mapImage);
	delete mapImage;
	delete mapImagepng;
	
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

	Gdiplus::Graphics *g = new Graphics(hMemDC);



	stage1MapView(hMemDC, g);



	print_Player(g); 
	print_bullet(g);
	print_Player_shadow(g);


	if (Inputsystem::getInstance().mou_Wheel)
		GameManager::getInstance().setScreenflag(1);

	GameManager::getInstance().show_FPS(hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y);

	ScreenManager::getInstance().SetCursor(hMemDC, mapinpoint.x, mapinpoint.y, ScreenManager::getInstance().cursor);

	if (Inputsystem::getInstance().mou_R)
	{
		TransparentBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x + 104, mapinpoint.y + 94, size.x - 104, size.y - 94, SRCCOPY);
	}
	else
	{
		BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y, SRCCOPY);
	}
	//	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, mapinpoint.x, mapinpoint.y, SRCCOPY);
	//	BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
	delete g;


	ReleaseDC(g_hWnd, hdc);
}

void GameScreen::mapImageview(HDC hdc, Graphics* g)
{
	//HDC hmemDC = CreateCompatibleDC(hdc);
	//HBITMAP oldbitmap = (HBITMAP)SelectObject(hmemDC, *mapImage);

	//BitBlt(hdc, 0, 0, 2320, 1580, hmemDC, mapoutpoint.x, mapoutpoint.y, SRCCOPY);

	//SelectObject(hmemDC, oldbitmap);

	g->DrawImage(mapImagepng, 0, 0, mapoutpoint.x * 100, mapoutpoint.y * 100, 2300, 1500, UnitPixel);


}

void GameScreen::stage1MapView(HDC hdc, Graphics *g)
{
	//for (int i = mapoutpoint.x; i < mapoutpoint.x + 23; i++)
	//	for (int j = mapoutpoint.y; j < mapoutpoint.y + 15; j++)
	//	{
	//		if (Map_Layer_1[i][j].in_use)
	//		{
	//			MapManager::getInstance().LoadTile(g, &Map_Layer_1[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

	//		}
	//	}


	//	mapImageview(hdc, g);


	for (int i = mapoutpoint.x; i < mapoutpoint.x + 23; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 15; j++)
		{
			if (Map_Layer_2[i][j].in_use)
			{
				MapManager::getInstance().LoadTile(g, &Map_Layer_2[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

			}
		}

	for (int i = 0; i < Vobstacle.size(); i++)
	{
		obstacle_info *temp = &Vobstacle[i];
		if (mapoutpoint.x <= temp->pos.x && temp->pos.x < mapoutpoint.x + 23 &&
			mapoutpoint.y <= temp->pos.y && temp->pos.y < mapoutpoint.y + 15)
		{
			MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
		}
	}

	//item
	for (int i = 0; i < Vitem.size(); i++)
	{
		item_info *temp = &Vitem[i];

		if (mapoutpoint.x <= temp->pos.x && temp->pos.x < mapoutpoint.x + 23 &&
			mapoutpoint.y <= temp->pos.y && temp->pos.y < mapoutpoint.y + 15)
		{
			if (Map_Layer_1[(*temp).pos.x][(*temp).pos.y].on_obj == 1)
				MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), -25 + 100 * ((*temp).pos.y - mapoutpoint.y), false);
			else
				MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
		}
	}

	//enemy
	for (int i = 0; i < Venemy.size(); i++)
	{
		enemy_info *temp = &Venemy[i];
		if (mapoutpoint.x <= temp->pos.x && temp->pos.x < mapoutpoint.x + 23 &&
			mapoutpoint.y <= temp->pos.y && temp->pos.y < mapoutpoint.y + 15)
		{
			MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
		}

	}
}

void GameScreen::print_Player(Graphics *g)
{

	Image *img = GameManager::getInstance().returnimagepointer(player->col, player->row);
	POINT mouse = GameManager::getInstance().getmouse();

	Point pos = { mapinpoint.x + player->mappos.x - 54, mapinpoint.y + player->mappos.y -47 };


	if (mouse.x - player->mappos.x > 0)
	{
		g->DrawImage(img, pos.X, pos.Y);
	}
	else
	{
		g->DrawImage(img, pos.X + img->GetWidth(), pos.Y, -1 * img->GetWidth(), img->GetHeight());
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
			load_Image(g, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, false);
			load_Image(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 74, mapinpoint.y + player->mappos.y - 47, false);
			load_Image(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 94, mapinpoint.y + player->mappos.y - 47, false);
		}
		else
		{
			load_Image(g, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, true);
			load_Image(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 20, mapinpoint.y + player->mappos.y - 47, true);
			load_Image(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y - 47, true);
		}
	}
	break;
	}
	//총 구현

	Matrix mat;
	Image *gunimg = GameManager::getInstance().returnimagepointer(player->curgun->col, player->curgun->row);
	int gun_width = gunimg->GetWidth(), gun_height = gunimg->GetHeight();

	if (player->curgun->state == Gun::IDLE)
	{

		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

		mat.RotateAt(rotation % 360,
			Gdiplus::PointF(pos.X + gun_width / 2,
				pos.Y + 25 + gun_height / 2));
		g->SetTransform(&mat);

		if (mouse.x - player->mappos.x > 0)
		{
			g->DrawImage(gunimg, pos.X, pos.Y + 25);
		}
		else
		{
			g->DrawImage(gunimg,
				pos.X, pos.Y + 25 + gun_height,
				gun_width,
				-1 * gun_height);
		}
		mat.RotateAt((360 - rotation) % 360,
			Gdiplus::PointF(pos.X + gun_width / 2,
				pos.Y + 25 + gun_height / 2));
		g->SetTransform(&mat);
	}
	else
	{
		static int rebound = 10;

		if (rebound > 0)
		{
			rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

			float dis = get_distance(player->mappos, mouse);
			POINTF dir = { float(((mouse.x - (player->mappos.x)) * rebound) / dis),
						float(((mouse.y - (player->mappos.y)) * rebound) / dis) };

			mat.RotateAt(rotation % 360,
				Gdiplus::PointF(pos.X - dir.x +  gun_width / 2,
					pos.Y + 25 - dir.y + + gun_height / 2));
			g->SetTransform(&mat);

			if (mouse.x - player->mappos.x > 0)
			{
				g->DrawImage(gunimg, pos.X - dir.x, pos.Y + 25 - dir.y);
			}
			else
			{
				g->DrawImage(gunimg,
					INT(pos.X - dir.x), INT(pos.Y + 25 - dir.y + gun_height),
					gun_width,
					-1 * gun_height);
			}
			mat.RotateAt((360 - rotation) % 360,
				Gdiplus::PointF(pos.X + gun_width / 2,
					pos.Y + 25 + gun_height / 2));
			g->SetTransform(&mat);
			rebound -= 2;
		}
		else
		{
			rebound = 10;
			player->curgun->state = Gun::IDLE;
		}
	}

	//레이저 포인터
	static bool mou_R_up = false;
	if (Inputsystem::getInstance().mou_R)
	{

		POINT gun_core_point = { player->mappos.x, player->mappos.y};
		Point raiser_front_point = { player->mappos.x - 4, player->mappos.y + 24 };
		Point raiser_end_point = { player->mappos.x - 4, player->mappos.y + 24 };
		float dis = get_distance(gun_core_point, mouse);
		int angle_rotation = angle(gun_core_point, mouse);



		POINT cur = { gun_core_point.x, gun_core_point.y };
		Point dir = { INT(float(((mouse.x - (raiser_front_point.X)) * player->curgun->range) / dis)),
					INT(float(((mouse.y - (raiser_front_point.Y)) * player->curgun->range) / dis)) };
		Point dir2 = { INT(float(((mouse.x - (raiser_front_point.X)) * 20) / dis)),
					INT(float(((mouse.y - (raiser_front_point.Y)) * 20) / dis)) };

		raiser_front_point = raiser_front_point + dir2 + mapinpoint;
		raiser_end_point = raiser_end_point + dir + mapinpoint;

		Pen redpen(Color(255, 0, 0), 3);

		g->DrawLine(&redpen, raiser_front_point, raiser_end_point);
		g->DrawEllipse(&redpen, Rect(raiser_end_point.X - 2, raiser_end_point.Y - 2, 4, 4));

		if(player->state == Player::WALK)
			player->velocity = 5;

		ScreenManager::getInstance().cursor--;
		if (ScreenManager::getInstance().cursor < 0)
			ScreenManager::getInstance().cursor = 0;

		mou_R_up = true;
	}
	else if(mou_R_up)
	{
		ScreenManager::getInstance().cursor = 2;
		player->velocity = 10;
		mou_R_up = false;
	}

}

void GameScreen::print_bullet(Graphics* g)
{
	

	Image *img;
	GameManager::getInstance().returnimagepointer(player->col, player->row);
	for (int i = 0; i < player->bullet_using.size(); i++)
	{
		if (player->bullet_using[i]->state == Bullet::START)
		{
			img = GameManager::getInstance().returnimagepointer(player->bullet_using[i]->col, player->bullet_using[i]->row);
			Image *img2 = GameManager::getInstance().returnimagepointer(42, 5);
			Matrix mat;


			mat.RotateAt((player->bullet_using[i]->rotation) % 360,
				Gdiplus::PointF(INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100),
					INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100))));
			g->SetTransform(&mat);

			g->DrawImage(img, INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100) - (img->GetWidth() * 0.5),
				INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100) - (img->GetHeight()* 0.5)));

			g->DrawImage(img2, INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100) - (img->GetWidth() * 0.5),
				INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100) - (img->GetHeight()* 0.5)));


			mat.RotateAt((360 - (player->bullet_using[i]->rotation)) % 360,
				Gdiplus::PointF(INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100),
					INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100))));
			g->SetTransform(&mat);
			player->bullet_using[i]->state = Bullet::SHOOT;
		}
		else if(player->bullet_using[i]->state == Bullet::SHOOT)
		{
			img = GameManager::getInstance().returnimagepointer(player->bullet_using[i]->col, player->bullet_using[i]->row);
			Matrix mat;

			mat.RotateAt(((player->bullet_using[i]->rotation)) % 360,
				Gdiplus::PointF(INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100),
					INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100))));
			g->SetTransform(&mat);

			g->DrawImage(img, INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100) - (img->GetWidth() * 0.5),
				INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100) - (img->GetHeight()* 0.5)));


			mat.RotateAt((360 - (player->bullet_using[i]->rotation)) % 360,
				Gdiplus::PointF(INT(player->bullet_using[i]->cur.x) - (mapoutpoint.x * 100),
					INT(player->bullet_using[i]->cur.y - (mapoutpoint.y * 100))));
			g->SetTransform(&mat);
		}
		else
		{
			player->bullet_pool.erase(player->bullet_using.begin() + i);
			i--;
		}
	}
}

void GameScreen::print_Player_shadow(Graphics* g)
{

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

	Point pos = { mapinpoint.x + player->mappos.x - 54, mapinpoint.y + player->mappos.y - 47 }; // 캐릭터의 중심


	int width = img->GetWidth();
	int height = img->GetHeight();

	if (mouse.x - player->mappos.x > 0)
	{
		g->DrawImage(img, Rect(pos.X, pos.Y + 2 * height, width,  -1 * height),
			0, 0, width, height,
		UnitPixel, &imageAtt);
	}
	else
	{
		g->DrawImage(img, Rect(pos.X + width, pos.Y + 2 * height, -1 * width, -1 * height),
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
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, false);
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 74, mapinpoint.y + player->mappos.y - 47, false);
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 94, mapinpoint.y + player->mappos.y - 47, false);
		}
		else
		{
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(41, dash_effect++), dash_start.X - mouseinpoint.x, dash_start.Y, true);
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x - 20, mapinpoint.y + player->mappos.y - 47, true);
			load_Image_shadow(g, GameManager::getInstance().returnimagepointer(35, 0), mapinpoint.x + player->mappos.x, mapinpoint.y + player->mappos.y - 47, true);
		}
	}
	break;
	}
	//총 구현

	Matrix mat;
	Image *gunimg = GameManager::getInstance().returnimagepointer(player->curgun->col, player->curgun->row);
	int gun_width = gunimg->GetWidth(), gun_height = gunimg->GetHeight();

	if (player->curgun->state == Gun::IDLE)
	{

		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

		mat.RotateAt((360 - rotation) % 360,
			Gdiplus::PointF(pos.X + gun_width / 2, pos.Y + 25 + gun_height));
		g->SetTransform(&mat);

		if (mouse.x - player->mappos.x < 0)
		{
			g->DrawImage(gunimg, Rect(pos.X, pos.Y - 25 + gun_height, gun_width, gun_height),
				0, 0, gun_width, gun_height,
				UnitPixel, &imageAtt);
		}
		else
		{
			g->DrawImage(gunimg, Rect(pos.X, pos.Y - 25 + 2 * gun_height, gun_width, -1 * gun_height),
				0, 0, gun_width, gun_height,
				UnitPixel, &imageAtt);
		}

		mat.RotateAt(rotation % 360,
			Gdiplus::PointF(pos.X + gun_width / 2, pos.Y + 25 + gun_height));
		g->SetTransform(&mat);
	}
	else
	{
		static int rebound = 10;

		if (rebound > 0)
		{
			rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

			float dis = get_distance(player->mappos, mouse);
			POINTF dir = { float(((mouse.x - (player->mappos.x)) * rebound) / dis),
						float(((mouse.y - (player->mappos.y)) * rebound) / dis) };


			mat.RotateAt((360 - rotation) % 360,
				Gdiplus::PointF(pos.X - dir.x + gun_width / 2, pos.Y + dir.y + 25 + gun_height));
			g->SetTransform(&mat);

			if (mouse.x - player->mappos.x < 0)
			{
				g->DrawImage(gunimg, Rect(pos.X - dir.x, pos.Y - 25 + gun_height + dir.y, gun_width, gun_height),
					0, 0, gun_width, gun_height,
					UnitPixel, &imageAtt);
			}
			else
			{
				g->DrawImage(gunimg, Rect(pos.X - dir.x, pos.Y - 25 + 2 * gun_height + dir.y, gun_width, -1 * gun_height),
					0, 0, gun_width, gun_height,
					UnitPixel, &imageAtt);
			}

			mat.RotateAt(rotation % 360,
				Gdiplus::PointF(pos.X + gun_width / 2, pos.Y + 25 + gun_height));
			g->SetTransform(&mat);
			rebound -= 2;
		}
		else
		{
			rebound = 10;
			player->curgun->state = Gun::IDLE;
		}
	}
}

void GameScreen::load_Image(Graphics *g, Image *img, int a, int b, bool reverse)
{
	if (reverse)
	{
		g->DrawImage(img, a + img->GetWidth(), b, -1 * img->GetWidth(), img->GetHeight());

	}
	else
	{
		g->DrawImage(img, a, b);

	}
}

void GameScreen::load_Image_shadow(Graphics *g, Image * img, int a, int b, bool reverse)
{
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
		g->DrawImage(img, Rect(a + width, b + 2 * height, -1 * width, -1 * height),
			0, 0, width, height,
			UnitPixel, &imageAtt);

	}
	else
	{
		g->DrawImage(img, Rect(a, b + 2 * height, width, -1 * height),
			0, 0, width, height,
			UnitPixel, &imageAtt);

	}
}

void GameScreen::update()
{
	int velocity = player->velocity;
//	int velocity = 100;
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

			player->curgun->state = Gun::SHOOT;

			RECT rt;
			GetClientRect(g_hWnd, &rt);

			// dir 계산
			POINT gun_core_point = { player->mappos.x - 4, player->mappos.y + 28 };
			POINT gun_front_point = { player->mappos.x + 16, player->mappos.y + 28 };
			float dis = get_distance(gun_core_point, mouse);
			int angle_rotation = angle(gun_core_point, mouse);

			

			POINT cur = { gun_core_point.x, gun_core_point.y };
			POINTF dir = { float(((mouse.x - (gun_core_point.x)) * player->curgun->bullet_speed) / dis),
						float(((mouse.y - (gun_core_point.y)) * player->curgun->bullet_speed) / dis) };
			POINTF dir2 = { float(((mouse.x - (gun_core_point.x)) * 60) / dis),
						float(((mouse.y - (gun_core_point.y)) * 60) / dis) };
			cur.x += dir2.x;
			cur.y += dir2.y;
			cur.x += mapinpoint.x;
			cur.y += mapinpoint.y;
			cur.x += mapoutpoint.x * 100;
			cur.y += mapoutpoint.y * 100;
			rotate_dot(gun_core_point, cur, angle_rotation);

			player->shot_bullet(dir, cur, mapoutpoint, angle_rotation);
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
