
#include "framework.h"
#include "ScreenManager.h"
#include "GameManager.h"
#include "FileSystem.h"
#include "GM_Util.h"
#include "MapManager.h"
#include "Record.h"
#include "GameScreen.h"

void ScreenManager::Screen()
{

	int n = GameManager::getInstance().getScreen_flag();
	switch (n)
	{
	case 1:
	{
		MainMenu();
	}
	break;
	case 2:
	{
		char_sel();
	}
	break;
	case 3:
	{
		GameScreen::getInstance().init();
		GameManager::getInstance().setScreenflag(7);
	}
	break;
	case 4:
	{
		MapManager::getInstance().Edit_init();
		GameManager::getInstance().setScreenflag(5);
	}
	break;
	case 5:
	{
		MapManager::getInstance().TotalView();
	}
	break;
	case 6:
	{
		MapManager::getInstance().Edit_shutdown();
		GameManager::getInstance().setScreenflag(1);
	}
	break;
	case 7:
	{
		GameScreen::getInstance().TotalGameview();
	}
	break;
	case 8:
	{
		GameScreen::getInstance().shutdown();
		GameManager::getInstance().setScreenflag(1);
	}
	break;
	}


}

void ScreenManager::MainMenu()
{
	HDC hdc = GetDC(g_hWnd);

	
	POINT size = GameManager::getInstance().getsize();

	const int addsize = 100;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, size.x + addsize, size.y + addsize);
	HBITMAP hOldBitmap;
	int bx, by;
	//
	int msglen;
	
	bx = (*(GameManager::getInstance().menu)[0]).GetWidth();
	by = (*(GameManager::getInstance().menu)[0]).GetHeight();

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	bg_tile_x += 2;
	bg_tile_y += 2;
	if (bg_tile_x >= 98)
	{
		bg_tile_x = 0;
		bg_tile_y = 0;
	}
	//더블 버퍼링
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[0], addsize - bg_tile_x, addsize - bg_tile_y); // background
//	DoubleBuffering(hWnd, hMemDC, img[0], 0, 0);

	DoubleBufferingtile(hMemDC, (GameManager::getInstance().menu)[2], size.x + addsize, size.y + addsize); // tiled
	
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[1], 100 + addsize - bg_tile_x, 80 + addsize - bg_tile_y); // logo

	int toggle_x = -65, toggle_y = addsize + 400, toggle_y_interval = 150;
	POINT pos = GameManager::getInstance().getmouse();
	
	pos.x += addsize - bg_tile_x;
	pos.y += addsize - bg_tile_y;


	if (!is_in_rectangle(toggle_x + addsize - bg_tile_x, toggle_y + addsize - bg_tile_y - 20, toggle_x + addsize - bg_tile_x + 600, toggle_y + addsize - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x + addsize - bg_tile_x, toggle_y + addsize - bg_tile_y); // toggle 
		DoubleBufferingtext(hMemDC, _T("PLAY  ADVENTURE"), 50 + addsize - bg_tile_x, toggle_y + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0); // text
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x + addsize - bg_tile_x, toggle_y + addsize - bg_tile_y); // toggle 
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x + 1400 - bg_tile_x, toggle_y + addsize - bg_tile_y); // star toggle 
		DoubleBufferingtext(hMemDC, _T("PLAY  ADVENTURE"), 50 + addsize - bg_tile_x, toggle_y + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0), 0); // text
		if (Inputsystem::getInstance().mou_L)
			GameManager::getInstance().setScreenflag(2);
	}

	if (!is_in_rectangle(toggle_x * 2 + addsize - bg_tile_x, toggle_y + toggle_y_interval + addsize - bg_tile_y - 20, toggle_x * 2 + addsize - bg_tile_x + 600, toggle_y + toggle_y_interval + addsize - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 2 + addsize - bg_tile_x, toggle_y + toggle_y_interval + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("MAP  EDIT"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0);

	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 2 + addsize - bg_tile_x, toggle_y + toggle_y_interval + addsize - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 2 + 1400 - bg_tile_x, toggle_y + toggle_y_interval + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("MAP  EDIT"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0), 0);
		if (Inputsystem::getInstance().mou_L)
			GameManager::getInstance().setScreenflag(4);
	}

	if (!is_in_rectangle(toggle_x * 3 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 2 + addsize - bg_tile_y - 20, toggle_x * 3 + addsize - bg_tile_x + 600, toggle_y+ toggle_y_interval * 2 + addsize - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 3 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 2 + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("SETTING"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0);
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 3 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 2 + addsize - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 3 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 2 + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("SETTING"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0), 0);
	}
	if (!is_in_rectangle(toggle_x * 4 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 3 + addsize - bg_tile_y - 20, toggle_x * 4 + addsize - bg_tile_x + 600, toggle_y  + toggle_y_interval * 3 + addsize - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 4 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 3 + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("EXIT"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0);
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 4 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 3 + addsize - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 4 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 3 + addsize - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("EXIT"), 50 + addsize - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0), 0);
		if (Inputsystem::getInstance().mou_L)
		{
			GameManager::getInstance().ShutDown();
			exit(0);
		}
	}
	SetCursor(hMemDC, addsize - bg_tile_x, addsize - bg_tile_y, 0);

	GameManager::getInstance().show_FPS(hMemDC, addsize - bg_tile_x, addsize - bg_tile_y);

	BitBlt(hdc, 0, 0, bx, by, hMemDC, addsize - bg_tile_x, addsize - bg_tile_y, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);

	ReleaseDC(g_hWnd, hdc);

}

void ScreenManager::char_sel()
{
	HDC hdc = GetDC(g_hWnd);

	const int addsize = 100;
	static int char_select = 0;

	POINT size = GameManager::getInstance().getsize();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, size.x + addsize, size.y + addsize);
	HBITMAP hOldBitmap;
	int bx, by;
	//
	int msglen;

	bx = (*(GameManager::getInstance().menu)[0]).GetWidth();
	by = (*(GameManager::getInstance().menu)[0]).GetHeight();

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	bg_tile_x += 2;
	bg_tile_y += 2;
	if (bg_tile_x >= 98)
	{
		bg_tile_x = 0;
		bg_tile_y = 0;
	}
	//더블 버퍼링
	DoubleBufferingtile(hMemDC, (GameManager::getInstance().returnimagepointer(18, 0)), size.x + addsize, size.y + addsize); // background

	DoubleBufferingtile(hMemDC, (GameManager::getInstance().returnimagepointer(18, 1)), size.x + addsize, size.y + addsize); // tiled

	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 2)), addsize - bg_tile_x, 200 + addsize - bg_tile_y); // bgline

	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 3)), addsize - bg_tile_x, 50 + addsize - bg_tile_y); // bgtitle
	DoubleBufferingtext(hMemDC, _T("CHARACTER  SELECTION"), addsize - bg_tile_x + 50, 50 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0);

	POINT mouse = GameManager::getInstance().getmouse();

	mouse.x += addsize - bg_tile_x;
	mouse.y += addsize - bg_tile_y;


	

	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(19, 0)), addsize - bg_tile_x + 800, 150 + addsize - bg_tile_y); // card
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(19, 1)), addsize - bg_tile_x + 1200, 150 + addsize - bg_tile_y); // card
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(19, 2)), addsize - bg_tile_x + 400, 300 + addsize - bg_tile_y); // card
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(19, 3)), addsize - bg_tile_x + 800, 300 + addsize - bg_tile_y); // card
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(19, 4)), addsize - bg_tile_x + 1200, 300 + addsize - bg_tile_y); // card

	if (char_select == 1)
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(21, 1)), addsize - bg_tile_x + 400, 150 + addsize - bg_tile_y); // Jimmy card

		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 4)), addsize - bg_tile_x + 50, 450 + addsize - bg_tile_y);	// record

		TCHAR temp[100];

		Record::getInstance().get_record();
		_stprintf_s(temp, _T("%lu"), Record::getInstance().Play_Time);
		DoubleBufferingtext(hMemDC, _T("PLAY TIME"), addsize - bg_tile_x + 50, 460 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 460 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);


		_stprintf_s(temp, _T("%lu"), Record::getInstance().Kill);
		DoubleBufferingtext(hMemDC, _T("KILLS"), addsize - bg_tile_x + 50, 520 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 520 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);


		_stprintf_s(temp, _T("%lu"), Record::getInstance().Death);
		DoubleBufferingtext(hMemDC, _T("DEATHS"), addsize - bg_tile_x + 50, 580 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 580 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);


		_stprintf_s(temp, _T("%lu"), Record::getInstance().Boss_Kill);
		DoubleBufferingtext(hMemDC, _T("BOSS KILLS"), addsize - bg_tile_x + 50, 640 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 640 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);


		_stprintf_s(temp, _T("%lu"), Record::getInstance().Best_Run_Time);
		DoubleBufferingtext(hMemDC, _T("BEST RUN TIME"), addsize - bg_tile_x + 50, 700 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 700 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);


		_stprintf_s(temp, _T("%lu"), Record::getInstance().Longest_Run_Time);
		DoubleBufferingtext(hMemDC, _T("LONGEST RUN TIME"), addsize - bg_tile_x + 50, 760 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, temp, addsize - bg_tile_x + 270, 760 + addsize - bg_tile_y, 580, 60, 25, Color(255, 255, 255, 255), 0);



		//
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 5)), addsize - bg_tile_x + 650, 450 + addsize - bg_tile_y); // name
		DoubleBufferingtext(hMemDC, _T("JIMMY"), addsize - bg_tile_x + 670, 451 + addsize - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255), 0);

		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(21, 0)), addsize - bg_tile_x + 500, 450 + addsize - bg_tile_y); // char_card
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 6)), addsize - bg_tile_x + 800, 500 + addsize - bg_tile_y); // stat
		DoubleBufferingtext(hMemDC, _T("HEALTH"), addsize - bg_tile_x + 810, 520 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 0)), addsize - bg_tile_x + 960, 515 + addsize - bg_tile_y); // stat bar
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 1)), addsize - bg_tile_x + 990, 515 + addsize - bg_tile_y); // stat bar
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 2)), addsize - bg_tile_x + 1020, 515 + addsize - bg_tile_y); // stat bar

		DoubleBufferingtext(hMemDC, _T("SHIELD"), addsize - bg_tile_x + 810, 555 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 0)), addsize - bg_tile_x + 960, 550 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 1)), addsize - bg_tile_x + 990, 550 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 2)), addsize - bg_tile_x + 1020, 550 + addsize - bg_tile_y); // stat
		DoubleBufferingtext(hMemDC, _T("MELEE"), addsize - bg_tile_x + 810, 590 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 0)), addsize - bg_tile_x + 960, 585 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 1)), addsize - bg_tile_x + 990, 585 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 2)), addsize - bg_tile_x + 1020, 585 + addsize - bg_tile_y); // stat
		DoubleBufferingtext(hMemDC, _T("STAMINA"), addsize - bg_tile_x + 810, 625 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 0)), addsize - bg_tile_x + 960, 620 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 1)), addsize - bg_tile_x + 990, 620 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 2)), addsize - bg_tile_x + 1020, 620 + addsize - bg_tile_y); // stat
		DoubleBufferingtext(hMemDC, _T("SPEED"), addsize - bg_tile_x + 810, 660 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 0)), addsize - bg_tile_x + 960, 655 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 1)), addsize - bg_tile_x + 990, 655 + addsize - bg_tile_y); // stat
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(20, 2)), addsize - bg_tile_x + 1020, 655 + addsize - bg_tile_y); // stat

		DoubleBufferingtext(hMemDC, _T("ALL_AROUND"), addsize - bg_tile_x + 810, 710 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, _T("CHARACTER. SKILLED"), addsize - bg_tile_x + 810, 745 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, _T("WITH ALL WEAPON"), addsize - bg_tile_x + 810, 780 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);
		DoubleBufferingtext(hMemDC, _T("TYPE."), addsize - bg_tile_x + 810, 815 + addsize - bg_tile_y, 580, 60, 20, Color(255, 255, 255, 255), 0);

		if (addsize - bg_tile_x + 610 <= mouse.x && mouse.x <= addsize - bg_tile_x + 910 &&
			860 + addsize - bg_tile_y <= mouse.y && mouse.y <= 910 + addsize - bg_tile_y)
		{
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 8)), addsize - bg_tile_x + 610, 860 + addsize - bg_tile_y); // TOGGLE
			DoubleBufferingtext(hMemDC, _T("START"), addsize - bg_tile_x + 660, 855 + addsize - bg_tile_y, 300, 50, 40, Color(255, 0, 255, 255), 0);
			if (Inputsystem::getInstance().mou_L)
			{
				Record::getInstance().oldTime = GetTickCount();
				GameManager::getInstance().setScreenflag(3);
			}
		}
		else
		{
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 7)), addsize - bg_tile_x + 610, 860 + addsize - bg_tile_y); // TOGGLE
			DoubleBufferingtext(hMemDC, _T("START"), addsize - bg_tile_x + 660, 855 + addsize - bg_tile_y, 300, 50, 40, Color(255, 0, 255, 255), 0);
			
		}
		
		if (addsize - bg_tile_x + 860 <= mouse.x && mouse.x <= addsize - bg_tile_x + 1160 &&
			915 + addsize - bg_tile_y <= mouse.y && mouse.y <= 965 + addsize - bg_tile_y)
		{
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 8)), addsize - bg_tile_x + 810, 920 + addsize - bg_tile_y); // TOGGLE
			DoubleBufferingtext(hMemDC, _T("CHANGE"), addsize - bg_tile_x + 860, 915 + addsize - bg_tile_y, 300, 50, 40, Color(255, 0, 255, 255), 0);
			if (Inputsystem::getInstance().mou_L)
			{ 
				char_select = 0;
			}
		}
		else
		{
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(18, 7)), addsize - bg_tile_x + 810, 920 + addsize - bg_tile_y); // TOGGLE
			DoubleBufferingtext(hMemDC, _T("CHANGE"), addsize - bg_tile_x + 860, 915 + addsize - bg_tile_y, 300, 50, 40, Color(255, 0, 255, 255), 0);
		
		}


	}
	else
	{
		if (is_in_rectangle(addsize - bg_tile_x + 400, 150 + addsize - bg_tile_y, addsize - bg_tile_x + 800, 250 + addsize - bg_tile_y, mouse))
		{
			static int frame_cur = 0;
			frame_cur %= 20;
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(22, frame_cur++)), addsize - bg_tile_x + 400, 150 + addsize - bg_tile_y);
			if (Inputsystem::getInstance().mou_L)
			{
				char_select = 1;
			}
		}
		else
		{
			DoubleBufferingimage(hMemDC, (GameManager::getInstance().returnimagepointer(21, 2)), addsize - bg_tile_x + 400, 150 + addsize - bg_tile_y);
		}

	}









	SetCursor(hMemDC, addsize - bg_tile_x, addsize - bg_tile_y, 0);


	BitBlt(hdc, 0, 0, bx, by, hMemDC, addsize - bg_tile_x, addsize - bg_tile_y, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
	GameManager::getInstance().show_FPS(hdc, 0, 0);

	ReleaseDC(g_hWnd, hdc);
}

void ScreenManager::DoubleBufferingimage(HDC hdc, Image * img, int x, int y)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	int bx, by;
	bx = (*img).GetWidth();
	by = (*img).GetHeight();
	
	g.DrawImage(img, x, y, bx, by);


}

void ScreenManager::DoubleBufferingtile(HDC hdc, Image * img, int bx, int  by)
{

	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	Gdiplus::TextureBrush tBrush(img);
	Status stat;
	stat = g.FillRectangle(&tBrush, Rect(0, 0, bx, by));


}

void ScreenManager::DoubleBufferingtext(HDC hdc, const TCHAR *str, int x, int y, int r, int b, int size, Color c, int kind)
{
//	MYRIADPRO
	PrivateFontCollection PFC;
	FontFamily fontFamily;
	switch (kind)
	{
	case 0:
		PFC.AddFontFile(_T("Resource\\FONT\\MYRIADPRO.OTF"));
		break;
	case 1:
		PFC.AddFontFile(_T("Resource\\FONT\\Maplestory_Bold.ttf"));
		break;
	case 2:
		PFC.AddFontFile(_T("Resource\\FONT\\Maplestory_Light.ttf"));
		break;
	}
	int numFamilies;
	PFC.GetFamilies(1, &fontFamily, &numFamilies);
//	FontFamily   fontFamily(L"Arial");
	Font         font(&fontFamily, size, FontStyleBold, UnitPoint);
	RectF        rectF(x, y, x + r, y + b);
	Gdiplus::SolidBrush   solidBrush(c);
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	g.DrawString(str, -1, &font, rectF, NULL, &solidBrush);

}

void ScreenManager::SetCursor(HDC hdc, int a, int b, int i) // a b : HDC - DC 크기 
{
	POINT mouse = GameManager::getInstance().getmouse();
	
	Image *img;
	switch (i)
	{
	case 1:
		img = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur32.png");
		break;
	case 2:
		img = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur52.png");
		break;
	case 3:
		img = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur72.png");
		break;
	case 4:
		img = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur92.png");
		break;
	default:
		img = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur32.png");
		break;

	}

	int x = img->GetWidth(), y = img->GetHeight();
	Graphics g(hdc);
	g.DrawImage(img,a + mouse.x - (x * 0.5),b + mouse.y - (y * 0.5), x, y);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	
		delete img;



}

