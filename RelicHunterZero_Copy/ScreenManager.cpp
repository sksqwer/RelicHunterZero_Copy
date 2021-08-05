
#include "framework.h"
#include "ScreenManager.h"
#include "GameManager.h"
#include "FileSystem.h"
#include "GM_Util.h"
#include "MapManager.h"

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
		MainMenu();
	}
	break;
	case 3:
	{
		MainMenu();
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
	}


}

void ScreenManager::MainMenu()
{
	HDC hdc = GetDC(g_hWnd);

	
	POINT size = GameManager::getInstance().getsize();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, size.x + 500, size.y + 500);
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
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[0], 500 - bg_tile_x, 500 - bg_tile_y); // background
//	DoubleBuffering(hWnd, hMemDC, img[0], 0, 0);

	DoubleBufferingtile(hMemDC, (GameManager::getInstance().menu)[2], bg_tile_x, bg_tile_y, bx, by); // tiled
	
	DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[1], 100 + 500 - bg_tile_x, 80 + 500 - bg_tile_y); // logo

	int toggle_x = -65, toggle_y = 500, toggle_y_interval = 150;
	POINT pos = GameManager::getInstance().getmouse();
	
	pos.x += 500 - bg_tile_x;
	pos.y += 500 - bg_tile_y;


	if (!is_in_rectangle(toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y - 20, toggle_x + 500 - bg_tile_x + 600, toggle_y + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y); // toggle 
		DoubleBufferingtext(hMemDC, _T("PLAY  ADVENTURE"), 50 + 500 - bg_tile_x, toggle_y + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255)); // text
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y); // toggle 
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x + 1400 - bg_tile_x, toggle_y + 500 - bg_tile_y); // star toggle 
		DoubleBufferingtext(hMemDC, _T("PLAY  ADVENTURE"), 50 + 500 - bg_tile_x, toggle_y + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0)); // text
	}

	if (!is_in_rectangle(toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y - 20, toggle_x * 2 + 500 - bg_tile_x + 600, toggle_y + toggle_y_interval + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("MAP  EDIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255));

	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 2 + 1400 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("MAP  EDIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0));
		if (Inputsystem::getInstance().mou_L)
			GameManager::getInstance().setScreenflag(4);
	}

	if (!is_in_rectangle(toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y - 20, toggle_x * 3 + 500 - bg_tile_x + 600, toggle_y+ toggle_y_interval * 2 + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("SETTING"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255));
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 3 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("SETTING"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0));
	}
	if (!is_in_rectangle(toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y - 20, toggle_x * 4 + 500 - bg_tile_x + 600, toggle_y  + toggle_y_interval * 3 + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[3], toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("EXIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 255, 255));
	}
	else
	{
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[4], toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingimage(hMemDC, (GameManager::getInstance().menu)[5], toggle_x * 4 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingtext(hMemDC, _T("EXIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + 500 - bg_tile_y, 580, 60, 30, Color(255, 255, 0, 0));
		if (Inputsystem::getInstance().mou_L)
		{
			GameManager::getInstance().ShutDown();
			exit(0);
		}
	}
	SetCursor(hMemDC, 500 - bg_tile_x, 500 - bg_tile_y, 0);

	BitBlt(hdc, 0, 0, bx, by, hMemDC, 500 - bg_tile_x, 500 - bg_tile_y, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);

	ReleaseDC(g_hWnd, hdc);

}

void ScreenManager::DoubleBufferingimage(HDC hdc, Image * img, int x, int y)
{
	Graphics g(hdc);
	int bx, by;
	bx = (*img).GetWidth();
	by = (*img).GetHeight();
	
	g.DrawImage(img, x, y, bx, by);


}

void ScreenManager::DoubleBufferingtile(HDC hdc, Image * img, int x, int y, int cx, int  cy)
{

	Graphics g(hdc);
	Gdiplus::TextureBrush tBrush(img);
	Status stat;
	stat = g.FillRectangle(&tBrush, Rect(0, 0, cx + 500, cy + 500));


}

void ScreenManager::DoubleBufferingtext(HDC hdc, const TCHAR *str, int x, int y, int r, int b, int size, Color c)
{
//	MYRIADPRO
	PrivateFontCollection PFC;
	FontFamily fontFamily;
	PFC.AddFontFile(_T("MYRIADPRO.OTF"));
	int numFamilies;
	PFC.GetFamilies(1, &fontFamily, &numFamilies);
//	FontFamily   fontFamily(L"Arial");
	Font         font(&fontFamily, size, FontStyleBold, UnitPoint);
	RectF        rectF(x, y, x + r, y + b);
	Gdiplus::SolidBrush   solidBrush(c);
	Graphics g(hdc);

	g.DrawString(str, -1, &font, rectF, NULL, &solidBrush);

}

void ScreenManager::SetCursor(HDC hdc, int a, int b, int i) // a b : HDC - DC 크기 
{
	POINT mouse = GameManager::getInstance().getmouse();
	
	Image *img[4];
	img[0] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur32.png");
	img[1] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur52.png");
	img[2] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur72.png");
	img[3] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur92.png");

	int x = img[i]->GetWidth(), y = img[i]->GetHeight();
	Graphics g(hdc);
	g.DrawImage(img[i],a + mouse.x - (x * 0.5),b + mouse.y - (y * 0.5), x, y);
	
	for (int i = 0; i < 4; i++)
		delete img[i];



}

