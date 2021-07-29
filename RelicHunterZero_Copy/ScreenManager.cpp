
#include "framework.h"
#include "ScreenManager.h"
#include "GameManager.h"
#include "FileSystem.h"
#include "GM_Util.h"
#include "MapManager.h"

void ScreenManager::Screen(HWND hWnd, HDC hdc)
{
	int n = GameManager::getInstance().getScreen_flag();
	switch (n)
	{
	case 1:
	{
		MainMenu(hWnd, hdc);
	}
	break;
	case 2:
	{
		MainMenu(hWnd, hdc);
	}
	break;
	case 3:
	{
		MainMenu(hWnd, hdc);
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
		MapManager::getInstance().TotalView(hWnd, hdc);
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

void ScreenManager::MainMenu(HWND hWnd, HDC hdc)
{
	Filesystem f = Filesystem::getInstance();
	const int filestart = 0;
	const int filenum = 6;

	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	(f).getpath(filestart, path, filenum);

	POINT size = GameManager::getInstance().getsize();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, size.x + 500, size.y + 500);
	HBITMAP hOldBitmap;
	int bx, by;
	Image *img[filenum];
	int msglen;

	for (int i = 0; i < filenum; i++)
	{
//		_stprintf_s(Tpath[i], _T("%s"), path[i]);
		msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		img[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
	bx = (*img[0]).GetWidth();
	by = (*img[0]).GetHeight();

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	bg_tile_x += 2;
	bg_tile_y += 2;
	if (bg_tile_x >= 98)
	{
		bg_tile_x = 0;
		bg_tile_y = 0;
	}
	//더블 버퍼링
	DoubleBufferingimage(hWnd, hMemDC, img[0], 500 - bg_tile_x, 500 - bg_tile_y); // background
//	DoubleBuffering(hWnd, hMemDC, img[0], 0, 0);

	DoubleBufferingtile(hWnd, hMemDC, img[2], bg_tile_x, bg_tile_y, bx, by); // tiled
	
	DoubleBufferingimage(hWnd, hMemDC, img[1], 100 + 500 - bg_tile_x, 80 + 500 - bg_tile_y); // logo

	int toggle_x = -65, toggle_y = 500, toggle_y_interval = 150;
	POINT pos = GameManager::getInstance().getmouse();
	
	pos.x += 500 - bg_tile_x;
	pos.y += 500 - bg_tile_y;


	if (!is_in_rectangle(toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y - 20, toggle_x + 500 - bg_tile_x + 600, toggle_y + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y); // toggle 
		DoubleBufferingtext(hWnd, hMemDC, _T("PLAY  ADVENTURE"), 50 + 500 - bg_tile_x, toggle_y + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 255, 255)); // text
	}
	else
	{
		DoubleBufferingimage(hWnd, hMemDC, img[4], toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y); // toggle 
		DoubleBufferingimage(hWnd, hMemDC, img[5], toggle_x + 1400 - bg_tile_x, toggle_y + 500 - bg_tile_y); // star toggle 
		DoubleBufferingtext(hWnd, hMemDC, _T("PLAY  ADVENTURE"), 50 + 500 - bg_tile_x, toggle_y + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 0, 0)); // text
	}

	if (!is_in_rectangle(toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y - 20, toggle_x * 2 + 500 - bg_tile_x + 600, toggle_y + toggle_y_interval + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("MAP  EDIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 255, 255));

	}
	else
	{
		DoubleBufferingimage(hWnd, hMemDC, img[4], toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingimage(hWnd, hMemDC, img[5], toggle_x * 2 + 1400 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("MAP  EDIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 0, 0));
		if (Inputsystem::getInstance().mou_L)
			GameManager::getInstance().setScreenflag(4);
	}

	if (!is_in_rectangle(toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y - 20, toggle_x * 3 + 500 - bg_tile_x + 600, toggle_y+ toggle_y_interval * 2 + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("SETTING"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 255, 255));
	}
	else
	{
		DoubleBufferingimage(hWnd, hMemDC, img[4], toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingimage(hWnd, hMemDC, img[5], toggle_x * 3 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("SETTING"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 0, 0));
	}
	if (!is_in_rectangle(toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y - 20, toggle_x * 4 + 500 - bg_tile_x + 600, toggle_y  + toggle_y_interval * 3 + 500 - bg_tile_y + 60 + 20, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("EXIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 255, 255));
	}
	else
	{
		DoubleBufferingimage(hWnd, hMemDC, img[4], toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingimage(hWnd, hMemDC, img[5], toggle_x * 4 + 1400 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("EXIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 10 + 500 - bg_tile_y, 580, 60, Color(255, 255, 0, 0));
		if (Inputsystem::getInstance().mou_L)
			exit(0);
	}

	SetCursor(hWnd, hMemDC, 500 - bg_tile_x, 500 - bg_tile_y, 0);

	BitBlt(hdc, 0, 0, bx, by, hMemDC, 500 - bg_tile_x, 500 - bg_tile_y, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
	for (int i = 0; i < filenum; i++)
	{
		delete img[i];
	}

}

void ScreenManager::DoubleBufferingimage(HWND hWnd, HDC hdc, Image * img, int x, int y)
{
	Graphics g(hdc);
	int bx, by;
	bx = (*img).GetWidth();
	by = (*img).GetHeight();
	
	g.DrawImage(img, x, y, bx, by);


}

void ScreenManager::DoubleBufferingtile(HWND hWnd, HDC hdc, Image * img, int x, int y, int cx, int  cy)
{

	Graphics g(hdc);
	TextureBrush tBrush(img);
	Status stat;
	stat = g.FillRectangle(&tBrush, Rect(0, 0, cx + 500, cy + 500));


}

void ScreenManager::DoubleBufferingtext(HWND hWnd, HDC hdc, const TCHAR *str, int x, int y, int r, int b, Color c)
{
//	MYRIADPRO
	PrivateFontCollection PFC;
	FontFamily fontFamily;
	PFC.AddFontFile(_T("MYRIADPRO.OTF"));
	int numFamilies;
	PFC.GetFamilies(1, &fontFamily, &numFamilies);
//	FontFamily   fontFamily(L"Arial");
	Font         font(&fontFamily, 30, FontStyleBold, UnitPoint);
	RectF        rectF(x, y, x + r, y + b);
	SolidBrush   solidBrush(c);
	Graphics g(hdc);

	g.DrawString(str, -1, &font, rectF, NULL, &solidBrush);

}

void ScreenManager::SetCursor(HWND hWnd, HDC hdc, int a, int b, int i) // a b : HDC - DC 크기 
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

