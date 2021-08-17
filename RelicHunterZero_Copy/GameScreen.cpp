#include "GameScreen.h"

#include "GameManager.h"
#include "Mapmanager.h"
#include "GM_Util.h"
#include "gun.h"
#include "bullet.h"
#include "casing.h"

void GameScreen::init()
{
	mapImage = new HBITMAP;
	*mapImage = (HBITMAP)LoadImage(NULL, L"Resource/Save/stage1.png",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	mapImagepng = Gdiplus::Image::FromFile(L"Resource/Save/stage12.png");

	Map_Layer_1 = new MapObject_ingame*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_1[i] = new MapObject_ingame[mapsize];

	Map_Layer_2 = new MapObject_ingame*[mapsize];
	for (int i = 0; i < mapsize; i++)
		Map_Layer_2[i] = new MapObject_ingame[mapsize];
	Vobstacle.clear();
	Vitem.clear();
	Venemy.clear();

	MapManager::getInstance().load_map(Map_Layer_1, Map_Layer_2, Vobstacle, Vitem, Venemy, stage1);

	//vector 정보 maplayer1에 저장

	Image *img;
	for (int i = 0; i < Vobstacle.size(); i++)
	{
		img = GameManager::getInstance().returnimagepointer(Vobstacle[i].col, Vobstacle[i].row);
		Vobstacle[i].size = { LONG(img->GetWidth()), LONG(img->GetHeight()) };
		Vobstacle[i].center = { LONG(img->GetWidth()) / 2, 100 - LONG(img->GetHeight()) / 2 };
		Map_Layer_1[Vobstacle[i].pos.x][Vobstacle[i].pos.y].obs = &Vobstacle[i];
	}

	for (int i = 0; i < Vitem.size(); i++)
	{
		img = GameManager::getInstance().returnimagepointer(Vitem[i].col, Vitem[i].row);
		Vitem[i].size = { LONG(img->GetWidth()), LONG(img->GetHeight()) };
		Map_Layer_1[Vitem[i].pos.x][Vitem[i].pos.y].item = &Vitem[i];
	}

	for (int i = 0; i < Venemy.size(); i++)
	{
		img = GameManager::getInstance().returnimagepointer(Venemy[i].col, Venemy[i].row);
		Venemy[i].size = { LONG(img->GetWidth()), LONG(img->GetHeight()) };
		Venemy[i].center = { LONG(img->GetWidth()) / 2, 100 - LONG(img->GetHeight()) / 2 };
		Map_Layer_1[Venemy[i].pos.x][Venemy[i].pos.y].enemy = &Venemy[i];
	}


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
	print_casing(g);
	print_Player_shadow(g);
	view_item_use(g);

	if (Inputsystem::getInstance().mou_Wheel)
		GameManager::getInstance().setScreenflag(1);

	GameManager::getInstance().show_FPS(hMemDC, mapinpoint.x + mouseinpoint.x, mapinpoint.y + mouseinpoint.y);

	ScreenManager::getInstance().SetCursor(hMemDC, mapinpoint.x, mapinpoint.y, ScreenManager::getInstance().cursor);

	POINT zoompoint = mapinpoint;
	if (zoompoint.x < 0)
		zoompoint.x = 0;
	if (zoompoint.y < 0)
		zoompoint.y = 0;
	if (Inputsystem::getInstance().mou_R)
	{
		TransparentBlt(hdc, 0, 0, size.x, size.y, hMemDC, zoompoint.x, zoompoint.y, size.x - 104, size.y - 94, SRCCOPY);
	}
	else
	{
		BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, zoompoint.x + mouseinpoint.x, zoompoint.y + mouseinpoint.y, SRCCOPY);
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
	for (int i = mapoutpoint.x; i < mapoutpoint.x + 23; i++)
		for (int j = mapoutpoint.y; j < mapoutpoint.y + 15; j++)
		{
			if (Map_Layer_1[i][j].in_use)
			{
				MapManager::getInstance().LoadTile(g, &Map_Layer_1[i][j], 100 * (i - mapoutpoint.x), 100 * (j - mapoutpoint.y), false);

			}
		}

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
		obstacle_ingame *temp = &Vobstacle[i];
		if (mapoutpoint.x <= temp->pos.x && temp->pos.x < mapoutpoint.x + 23 &&
			mapoutpoint.y <= temp->pos.y && temp->pos.y < mapoutpoint.y + 15)
		{
			MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);
		}
	}

	//item
	for (int i = 0; i < Vitem.size(); i++)
	{
		item_ingame *temp = &Vitem[i];

		if (mapoutpoint.x <= temp->pos.x && temp->pos.x < mapoutpoint.x + 23 &&
			mapoutpoint.y <= temp->pos.y && temp->pos.y < mapoutpoint.y + 15)
		{
			//test
			if (i == 12)
			{
				temp->pos = temp->pos;
			}
			if (temp->activate == false)
			{
				temp->col = temp->col;
			}
			if (Map_Layer_1[(*temp).pos.x][(*temp).pos.y].on_obj == 1)
				MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), -25 + 100 * ((*temp).pos.y - mapoutpoint.y), false);
			else
				MapManager::getInstance().LoadTile(g, temp, 100 * ((*temp).pos.x - mapoutpoint.x), 100 * ((*temp).pos.y - mapoutpoint.y), false);

			if (temp->is_ani && !temp->activate)
			{
				if (temp->row == 0)
				{
					if (temp->kind == item_info::HEAL || temp->kind == item_info::BIGHEAL || temp->kind == item_info::SHIELD)
					{
						Map_Layer_1[(*temp).pos.x][(*temp).pos.y].item = NULL;
						Vitem.erase(Vitem.begin() + i);
						i--;
					}
					else
					{
						temp->is_ani = false;
						temp->row = temp->max_frame - 1;
					}
				}
				
			}
		}
	}

	//enemy
	for (int i = 0; i < Venemy.size(); i++)
	{
		enemy_ingame *temp = &Venemy[i];
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

		rotation = angle(POINT({ player->mappos.x,  player->mappos.y }), { mouse.x, mouse.y });

		float dis = get_distance(player->mappos, mouse);
		POINTF dir = { float(((mouse.x - (player->mappos.x)) * rebound) / dis),
					float(((mouse.y - (player->mappos.y)) * rebound) / dis) };

		mat.RotateAt(rotation % 360,
			Gdiplus::PointF(pos.X - dir.x + gun_width / 2,
				pos.Y + 25 - dir.y + +gun_height / 2));
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

		if (rebound == 0)
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
		POINTF dir = { (float(((mouse.x - (raiser_front_point.X))) * 10/ dis)),
					(float(((mouse.y - (raiser_front_point.Y))) * 10 / dis)) };
		Point dir2 = { INT(float(((mouse.x - (raiser_front_point.X)) * 20) / dis)),
					INT(float(((mouse.y - (raiser_front_point.Y)) * 20) / dis)) };
		POINTF dir3 = { (float(((mouse.x - (raiser_front_point.X))) * 2 / dis)),
					(float(((mouse.y - (raiser_front_point.Y))) * 2 / dis)) };

		raiser_front_point = raiser_front_point + dir2 + mapinpoint;
		raiser_front_point.X += mapoutpoint.x * 100;
		raiser_front_point.Y += mapoutpoint.y * 100;

		POINTF endF;
		POINT end;
		end.x = raiser_end_point.X + dir.x + mapinpoint.x + mapoutpoint.x * 100;
		endF.x = end.x;
		end.y = raiser_end_point.Y + dir.y + mapinpoint.y + mapoutpoint.y * 100;
		endF.y = end.y;
		while (!collide_dot_IN_layer1(end, dir3,
			Map_Layer_1[(end.x / 100)][(end.y / 100)]) &&
			get_distance(end, POINT{ raiser_front_point.X, raiser_front_point.Y }) <= player->curgun->range + 28)
		{
			endF.x += dir.x;
			endF.y += dir.y;
			end.x = endF.x;
			end.y = endF.y;

		}

		raiser_front_point.X -= mapoutpoint.x * 100;
		raiser_front_point.Y -= mapoutpoint.y * 100;
		raiser_end_point.X = end.x - mapoutpoint.x * 100;
		raiser_end_point.Y = end.y - mapoutpoint.y * 100;

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
			static int bullet_effect_row = 0;

			if (bullet_effect_row < 3)
			{
				img = GameManager::getInstance().returnimagepointer(player->bullet_using[i]->bullet_effect_col, bullet_effect_row++);
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
				player->bullet_using.erase(player->bullet_using.begin() + i);
				i--;
				bullet_effect_row = 0;
			}

		}
	}
}

void GameScreen::print_casing(Graphics * g)
{
	Image *img;
	for (int i = 0; i < player->casing_using.size(); i++)
	{
		img = GameManager::getInstance().returnimagepointer(player->casing_using[i]->col, player->casing_using[i]->row);
		Matrix mat;

		mat.RotateAt(((player->casing_using[i]->rotation)) % 360,
			Gdiplus::PointF(INT(player->casing_using[i]->pos.x - (mapoutpoint.x * 100)),
				INT(player->casing_using[i]->pos.y - player->casing_using[i]->pos_z - (mapoutpoint.y * 100))));
		g->SetTransform(&mat); 
		
		g->DrawImage(img, INT(player->casing_using[i]->pos.x) - (img->GetWidth() * 0.5) - (mapoutpoint.x * 100),
			INT(player->casing_using[i]->pos.y - player->casing_using[i]-> pos_z - (img->GetHeight()* 0.5) - (mapoutpoint.y * 100)));


		mat.RotateAt((360 - (player->casing_using[i]->rotation)) % 360,
			Gdiplus::PointF(INT(player->casing_using[i]->pos.x - (mapoutpoint.x * 100)),
				INT(player->casing_using[i]->pos.y - player->casing_using[i]->pos_z - (mapoutpoint.y * 100))));
		g->SetTransform(&mat);
	}

	for (int i = 0; i < player->casing_using.size(); i++)
	{
		img = GameManager::getInstance().returnimagepointer(player->casing_using[i]->col, 3);

		Matrix mat;

		mat.RotateAt(((player->casing_using[i]->rotation)) % 360,
			Gdiplus::PointF(INT(player->casing_using[i]->pos.x - (mapoutpoint.x * 100)),
				INT(player->casing_using[i]->pos.y - (mapoutpoint.y * 100))));
		g->SetTransform(&mat);

		g->DrawImage(img, INT(player->casing_using[i]->pos.x) - (img->GetWidth() * 0.5) - (mapoutpoint.x * 100),
			INT(player->casing_using[i]->pos.y - (img->GetHeight()* 0.5) - (mapoutpoint.y * 100)));


		mat.RotateAt((360 - (player->casing_using[i]->rotation)) % 360,
			Gdiplus::PointF(INT(player->casing_using[i]->pos.x - (mapoutpoint.x * 100)),
				INT(player->casing_using[i]->pos.y - (mapoutpoint.y * 100))));
		g->SetTransform(&mat);
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

		if (rebound == 0)
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

void GameScreen::view_item_use(Graphics * g)
{
	const POINT enum_pos[5] = { {0, 0}, {0, -1}, {-1, 0}, {1, 0}, {0, 1}, };
	static int prompt_e_row = 0;
	static int Layer_row = 0;
	static int Layer_col = 0;
	POINT mouse = GameManager::getInstance().getmouse();
	MapObject_ingame *map;
	static DWORD oldTime = GetTickCount();
	if (get_distance(mouse, player->mappos) <= 150)
	{
		map = &Map_Layer_1[(mouse.x + mapinpoint.x) / 100 + mapoutpoint.x]
			[(mouse.y + mapinpoint.y) / 100 + mapoutpoint.y];
		if (map->item && map->item->activate)
		{

			if (Layer_row != (mouse.x + mapinpoint.x) / 100 + mapoutpoint.x ||
				Layer_col != (mouse.y + mapinpoint.y) / 100 + mapoutpoint.y)
			{
				Layer_row = (mouse.x + mapinpoint.x) / 100 + mapoutpoint.x;
				Layer_col = (mouse.y + mapinpoint.y) / 100 + mapoutpoint.y;
				prompt_e_row = 0;
			}

			Image *img = GameManager::getInstance().returnimagepointer(46, prompt_e_row++);
			prompt_e_row %= 8;

			if (map->obs || map->attr == MapObject_ingame::BLOCK)
			{
				g->DrawImage(img, int(Layer_row * 100 - mapoutpoint.x * 100 + 50 - img->GetWidth() / 2),
					int(Layer_col * 100 - mapoutpoint.y * 100 - 25 - img->GetHeight() / 2));
			}
			else
			{
				g->DrawImage(img, int(Layer_row * 100 - mapoutpoint.x * 100 + 50 - img->GetWidth() / 2),
					int(Layer_col * 100 - mapoutpoint.y * 100 - img->GetHeight() / 2));
			}

			if (Inputsystem::getInstance().key_E)
			{
				DWORD newTime = GetTickCount();
				if (newTime - oldTime < 500) return;
				oldTime = newTime;

				interaction_item(map->item);
			}

			return;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		map = &Map_Layer_1[(player->mappos.x + mapinpoint.x ) / 100 + mapoutpoint.x + enum_pos[i].x]
			[(player->mappos.y + mapinpoint.y) / 100 + mapoutpoint.y + enum_pos[i].y];
		if (map->item && map->item->activate)
		{

			if (Layer_row != (player->mappos.x + mapinpoint.x) / 100 + mapoutpoint.x + enum_pos[i].x ||
				Layer_col != (player->mappos.y + mapinpoint.y) / 100 + mapoutpoint.y + enum_pos[i].y)
			{
				Layer_row = (player->mappos.x + mapinpoint.x) / 100 + mapoutpoint.x + enum_pos[i].x;
				Layer_col = (player->mappos.y + mapinpoint.y) / 100 + mapoutpoint.y + enum_pos[i].y;
				prompt_e_row = 0;
			}

			Image *img = GameManager::getInstance().returnimagepointer(46, prompt_e_row++);
			prompt_e_row %= 8;

			if (map->obs || map->attr == MapObject_ingame::BLOCK)
			{
				g->DrawImage(img, int(Layer_row * 100 - mapoutpoint.x * 100 + 50 - img->GetWidth() / 2),
					int(Layer_col * 100 - mapoutpoint.y * 100 - 25 - img->GetHeight() / 2));
			}
			else
			{
				g->DrawImage(img, int(Layer_row * 100 - mapoutpoint.x * 100 + 50 - img->GetWidth() / 2),
					int(Layer_col * 100 - mapoutpoint.y * 100 - img->GetHeight() / 2));
			}

			if (Inputsystem::getInstance().key_E)
			{
				DWORD newTime = GetTickCount();
				if (newTime - oldTime < 500) return;
				oldTime = newTime;

				interaction_item(map->item);
			}

			break;
		}
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
	else if (!Inputsystem::getInstance().key_S && !Inputsystem::getInstance().key_W && player->state == Player::DASH)
	{
		if (GameManager::getInstance().getmouse().x >= player->mappos.x)
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
		else
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
	//bullet 총발사
	update_bullet();
	update_casing();
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
			cur = rotate_dot(gun_core_point, cur, angle_rotation);
			cur.x += mapinpoint.x;
			cur.y += mapinpoint.y;
			cur.x += mapoutpoint.x * 100;
			cur.y += mapoutpoint.y * 100;

			player->shot_bullet(dir, cur, mapoutpoint, angle_rotation);
	// 탄피
			int random = rand() % 20;
			if (rand() % 2)
				random *= -1;
			POINTF curF;
			curF.x = gun_core_point.x + dir.x * -1 / player->curgun->bullet_speed;
			curF.y = gun_core_point.y + dir.y * -1 / player->curgun->bullet_speed;
//			curF = rotate_dot(gun_core_point, curF, random);

			dir = { float(curF.x - gun_core_point.x),
						float(curF.y - gun_core_point.y) };

			curF.x = gun_core_point.x + dir.x * 10;
			curF.y = gun_core_point.y + dir.y * 10;

			random = rand() % 30;
			random += 50;

			curF.x += mapinpoint.x;
			curF.y += mapinpoint.y;
			curF.x += mapoutpoint.x * 100;
			curF.y += mapoutpoint.y * 100;
			player->shot_casing(curF, dir, 50, random, player->curgun->ret_bullet_type());
		}
	}

	player->update();
}

void GameScreen::update_bullet()
{
	for (int i = 0; i < player->bullet_using.size(); i++)
	{
		if (player->bullet_using[i]->state == Bullet::DESTROY)
			continue;
		POINTF d;
		POINTF cur_position;
		cur_position.x = player->bullet_using[i]->cur.x;
		cur_position.y = player->bullet_using[i]->cur.y;
		int speed = 0;
		d.x = player->bullet_using[i]->dir.x / player->bullet_using[i]->bullet_speed;
		d.y = player->bullet_using[i]->dir.y / player->bullet_using[i]->bullet_speed;
		while (speed <= player->bullet_using[i]->bullet_speed)
		{
			cur_position.x += d.x;
			cur_position.y += d.y;
			speed++;
			player->bullet_using[i]->cur.x = cur_position.x;
			player->bullet_using[i]->cur.y = cur_position.y;

			if (Map_Layer_1[int(player->bullet_using[i]->cur.x) / 100][int(player->bullet_using[i]->cur.y) / 100].enemy)
			{
				// 적과 충돌
				if (collide_dot_IN_layer1(player->bullet_using[i]->cur, d, Map_Layer_1[int(player->bullet_using[i]->cur.x) / 100][int(player->bullet_using[i]->cur.y) / 100]))
				{
					player->bullet_using[i]->state = Bullet::DESTROY;
					break;
				}
			}
			else if (Map_Layer_1[int(player->bullet_using[i]->cur.x) / 100][int(player->bullet_using[i]->cur.y) / 100].obs)
			{
				// 장애물과 충돌
				if (collide_dot_IN_layer1(player->bullet_using[i]->cur, d, Map_Layer_1[int(player->bullet_using[i]->cur.x) / 100][int(player->bullet_using[i]->cur.y) / 100]))
				{
					player->bullet_using[i]->state = Bullet::DESTROY;
					break;
				}
			}
			else if (Map_Layer_1[int(player->bullet_using[i]->cur.x) / 100][int(player->bullet_using[i]->cur.y) / 100].attr == MapObject_info::BLOCK)
			{
				player->bullet_using[i]->state = Bullet::DESTROY;
				break;
			}
			else if (get_distance(player->bullet_using[i]->cur, player->bullet_using[i]->pre) >= player->curgun->range)
			{
				player->bullet_using[i]->state = Bullet::DESTROY;
				break;
			}
		
		}
	}
}

void GameScreen::update_casing()
{

	for (int i = 0; i < player->casing_using.size(); i++)
	{
		Casing *temp = player->casing_using[i];
		temp->rotation += rand() % 10 + 20;
		temp->rotation %= 360;


		temp->pos.x += temp->dir.x * temp->power;
		temp->pos.y += temp->dir.y * temp->power;
		
		if(temp->gravity < 0)
			temp->pos_z += temp->gravity * temp->power * 1.5;
		else
			temp->pos_z += temp->gravity * temp->power;

		if ((temp->pos_z > temp->max_pos_z && temp->gravity > 0) || (temp->pos_z < 0 && temp->gravity < 0))
		{
			if (temp->gravity < 0)
				temp->pos_z = 0;
			else
				temp->pos_z = temp->max_pos_z;
			temp->max_pos_z -= 10; 
			temp->gravity *= -1;
			temp->power -= 2;
		}

		if (temp->max_pos_z < 0 || temp->power <= 0)
		{
			player->casing_using.erase(player->casing_using.begin() + i);
			i--;
		}
		

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

bool GameScreen::collide_dot_IN_layer1(POINT &dot, POINTF dir, MapObject_ingame map)
{
	if (map.enemy)
	{
		enemy_ingame temp = *map.enemy;

		if (is_in_rectangle(temp.center.x - temp.size.x * 0.5 + mapinpoint.x + mapoutpoint.x * 100,
			temp.center.y - temp.size.y * 0.5 + mapinpoint.y + mapoutpoint.y * 100,
			temp.center.x + temp.size.x * 0.5 + mapinpoint.x + mapoutpoint.x * 100,
			temp.center.y + temp.size.y * 0.5 + mapinpoint.y + mapoutpoint.y * 100,
			dot.x + mapinpoint.x, dot.y + mapinpoint.y))
		{
			POINTF dot_temp = {FLOAT(dot.x), FLOAT(dot.y)};
			while (is_in_rectangle(temp.center.x - temp.size.x * 0.5 + mapinpoint.x + mapoutpoint.x * 100,
				temp.center.y - temp.size.y * 0.5 + mapinpoint.y + mapoutpoint.y * 100,
				temp.center.x + temp.size.x * 0.5 + mapinpoint.x + mapoutpoint.x * 100,
				temp.center.y + temp.size.y * 0.5 + mapinpoint.y + mapoutpoint.y * 100,
				dot_temp.x + mapinpoint.x, dot_temp.y + mapinpoint.y))
			{
				dot_temp.x -= dir.x;
				dot_temp.y -= dir.y;
			}

			dot.x = int(dot_temp.x);
			dot.y = int(dot_temp.y);
			return true;
		}
		else
		{
			return false;
		}

	}
	else if (map.obs)
	{
		obstacle_ingame temp = *map.obs;

		if (is_in_rectangle(temp.center.x - temp.size.x * 0.5 + temp.pos.x * 100,
			temp.center.y - temp.size.y * 0.5 + temp.pos.y * 100,
			temp.center.x + temp.size.x * 0.5 + temp.pos.x * 100,
			temp.center.y + temp.size.y * 0.5 + temp.pos.y * 100,
			dot.x , dot.y ))
		{
			POINTF dot_temp = { FLOAT(dot.x), FLOAT(dot.y) };
			while (is_in_rectangle(temp.center.x - temp.size.x * 0.5 + temp.pos.x * 100,
				temp.center.y - temp.size.y * 0.5 + temp.pos.y * 100,
				temp.center.x + temp.size.x * 0.5 + temp.pos.x * 100,
				temp.center.y + temp.size.y * 0.5 + temp.pos.y * 100,
				dot_temp.x , dot_temp.y ))
			{
				dot_temp.x -= dir.x;
				dot_temp.y -= dir.y;
			}

			dot.x = int(dot_temp.x);
			dot.y = int(dot_temp.y);
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (map.attr == MapObject_ingame::BLOCK)
	{
		POINTF dot_temp = { FLOAT(dot.x), FLOAT(dot.y)};
		while (is_in_rectangle(int(dot.x / 100) * 100,
			int(dot.y / 100) * 100,
			int(dot.x / 100) * 100 + 100,
			int(dot.y / 100) * 100 + 100,
			int(dot_temp.x), int(dot_temp.y)))
		{
			dot_temp.x -= dir.x;
			dot_temp.y -= dir.y;
		}

		dot.x = int(dot_temp.x);
		dot.y = int(dot_temp.y);
		return true;
	}


	return false;
}

void GameScreen::interaction_item(item_ingame* item)
{
	if (item->kind >= item_info::PISTOL && item->kind <= item_info::MACHINEGUN)
	{

	}
	else if (item->kind == item_info::GRENADE)
	{
		if (player->grenade < player->grenade_max)
		{
			item->activate = false;
			item->animation_set();
			player->grenade += 3;
			player->grenade %= player->grenade_max;
		}
	}
	else if (item->kind == item_info::HEAVYAMMO)
	{
		if (player->heavy_ammo < player->ammo_max)
		{
			item->activate = false;
			item->animation_set();
			player->heavy_ammo += 100;
			player->heavy_ammo %= player->ammo_max;
		}
	}
	else if (item->kind == item_info::MEDIUMAMMO)
	{
		if (player->medium_ammo < player->ammo_max)
		{
			item->activate = false;
			item->animation_set();
			player->medium_ammo += 100;
			player->medium_ammo %= player->ammo_max;
		}
	}
	else if (item->kind == item_info::LIGHTAMMO)
	{
		if (player->light_ammo < player->ammo_max)
		{
			item->activate = false;
			item->animation_set();
			player->light_ammo += 100;
			player->light_ammo %= player->ammo_max;
		}
	}
	else if (item->kind == item_info::HEAL)
	{
		if (player->HP < player->maxhp)
		{
			item->activate = false;
			item->animation_set();
			player->HP += 50;
			player->HP %= player->maxhp;
		}
	}
	else if (item->kind == item_info::BIGHEAL)
	{
		if (player->HP < player->maxhp)
		{
			item->activate = false;
			item->animation_set();
			player->HP += 100;
			player->HP %= player->maxhp;
		}
	}
	else if (item->kind == item_info::SHIELD)
	{
		item->activate = false;
		item->animation_set();
		player->maxshield += 40;
	}

}
