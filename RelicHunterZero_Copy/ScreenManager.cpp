#include "ScreenManager.h"
#include "GameManager.h"
#include "FileSystem.h"
#include "GM_Util.h"

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
	}
}

void ScreenManager::MainMenu(HWND hWnd, HDC hdc)
{
	Filesystem f = GameManager::getInstance().getfilemanager();
	const int num = 5;

	char path[num][100];
	TCHAR Tpath[num][100];

	(f).getpath(0, path, num);

	POINT rect = GameManager::getInstance().getrect();

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.x + 500, rect.y + 500);
	HBITMAP hOldBitmap;
	int bx, by;
	Image *img[num];
	int msglen;

	for (int i = 0; i < num; i++)
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

	if (!is_in_rectangle(toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y, toggle_x + 500 - bg_tile_x + 600, toggle_y + 500 - bg_tile_y + 60, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x + 500 - bg_tile_x, toggle_y + 500 - bg_tile_y); // toggle 
		DoubleBufferingtext(hWnd, hMemDC, _T("PLAY  ADVENTURE"), 50 + 500 - bg_tile_x, toggle_y + 10 + 500 - bg_tile_y, 580, 60); // text
	}
	else
	{

	}
	if (!is_in_rectangle(toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y, toggle_x * 2 + 500 - bg_tile_x + 600, toggle_y + toggle_y_interval + 500 - bg_tile_y + 60, pos))
	{
		DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 2 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 500 - bg_tile_y);
		DoubleBufferingtext(hWnd, hMemDC, _T("MAP  EDIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval + 10 + 500 - bg_tile_y, 580, 60);
	}
	else
	{

	}
	if (!is_in_rectangle(toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y, toggle_x * 3 + 500 - bg_tile_x + 600, toggle_y * 2 + toggle_y_interval + 500 - bg_tile_y + 60, pos))
	{
	DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 3 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 500 - bg_tile_y);
	DoubleBufferingtext(hWnd, hMemDC, _T("SETTING"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 2 + 10+ 500 - bg_tile_y, 580, 60);
	}
	else
	{

	}
	if (!is_in_rectangle(toggle_x * 4 + 500 - bg_tile_x, toggle_y * 3 + toggle_y_interval + 500 - bg_tile_y, toggle_x * 4 + 500 - bg_tile_x + 600, toggle_y * 3 + toggle_y_interval + 500 - bg_tile_y + 60, pos))
	{
	DoubleBufferingimage(hWnd, hMemDC, img[3], toggle_x * 4 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3 + 500 - bg_tile_y);
	DoubleBufferingtext(hWnd, hMemDC, _T("EXIT"), 50 + 500 - bg_tile_x, toggle_y + toggle_y_interval * 3  + 10 + 500 - bg_tile_y, 580, 60);
	}
	else
	{

	}

	BitBlt(hdc, 0, 0, bx, by, hMemDC, 500 - bg_tile_x, 500 - bg_tile_y, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hMemDC);

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

void ScreenManager::DoubleBufferingtext(HWND hWnd, HDC hdc, const TCHAR *str, int x, int y, int r, int b)
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
	SolidBrush   solidBrush(Color(255, 255, 255, 255));
	Graphics g(hdc);

	g.DrawString(str, -1, &font, rectF, NULL, &solidBrush);
}
