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

//	MainView(hWnd, hMemDC);


	SubView(hWnd, hMemDC);


	BitBlt(hdc, 0, 0, bx, by, hMemDC, bx, by, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);

}

void MapManager::MainView(HWND hWnd, HDC hdc)
{
	POINT size = GameManager::getInstance().getsize();
	int bx = 1000, by = size.y;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);


	BitBlt(hdc, 0, 0, bx, by, hMemDC, bx, by, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
}

void MapManager::SubView(HWND hWnd, HDC hdc)
{
	int msglen;
	POINT size = GameManager::getInstance().getsize();
	int bx = size.x - 1000, by = size.y;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, bx, by);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);

	Filesystem f = Filesystem::getInstance();
	const int filestart = 6;
	const int filenum = 1;

	char path[1][100];
	TCHAR Tpath[1][100];

	(f).getpath(filestart, path, filenum);


	msglen = MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, path[0], strlen(path[0]), Tpath[0], msglen);
	Tpath[0][msglen] = NULL;

	Image toggleimg(Tpath[0]);

	Graphics g(hMemDC);
	g.DrawImage(&toggleimg, 0, 0);
	if (Inputsystem::getInstance().mou_L)
		GameManager::getInstance().setScreenflag(6);



	BitBlt(hdc, 1000, 0, bx, by, hMemDC, bx, by, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hbitmap);
	DeleteDC(hMemDC);
}
