#include "GameManager.h"
#include "ScreenManager.h"
#include "FileSystem.h"

void GameManager::Init()
{
	GDI_Init();
	Load_Image();
}

void GameManager::ShutDown()
{

	for (int i = 0; i < sizeof(menu) / sizeof(Image *); i++)
	{
		if(menu[i] != nullptr)
			delete menu[i];
	}

	for (int i = 0; i < sizeof(maptoggle) / sizeof(Image *); i++)
	{
		if (maptoggle[i])
			delete maptoggle[i];
	}

	for (int i = 0; i < sizeof(ship) / sizeof(Image *); i++)
	{
		if (ship[i])
			delete ship[i];
	}

	for (int i = 0; i < sizeof(green) / sizeof(Image *); i++)
	{
		if (green[i])
			delete green[i];
	}

	for (int i = 0; i < sizeof(desert) / sizeof(Image *); i++)
	{
		if (desert[i])
			delete desert[i];
	}

	for (int i = 0; i < sizeof(volcano) / sizeof(Image *); i++)
	{
		if (volcano[i])
			delete volcano[i];
	}

	for (int i = 0; i < sizeof(obstacle) / sizeof(Image *); i++)
	{
		if (obstacle[i] != nullptr)
			delete obstacle[i];
	}

	GDI_ShutDown();
//	delete[] obstacle;
}

void GameManager::GDI_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);

//	if (&GameManager::getInstance() != this) exit(0);
}

void GameManager::GDI_ShutDown()
{
	GdiplusShutdown(g_GdiToken);
}

void GameManager::update()
{
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 30) return;
	oldTime = newTime;

	GetCursorPos(&mouse);

	Inputsystem::getInstance().update();
}

void GameManager::show()
{
	
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 30) return;
	oldTime = newTime;


	ScreenManager::getInstance().Screen();
}

void GameManager::Load_Image()
{
	load_menu();
	load_maptoggle();
	load_ship();
	load_green();
	load_desert();
	load_vocano();
	load_obstacle();
}

void GameManager::load_menu()
{
	const int CSVrow = 0;
	const int CSVcol = 0;
	const int filenum = sizeof(menu) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];


	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		menu[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_maptoggle()
{
	const int CSVrow = 1;
	const int CSVcol = 0;
	const int filenum = sizeof(maptoggle) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		maptoggle[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_ship()
{
	const int CSVrow = 2;
	const int CSVcol = 0;
	const int filenum = sizeof(ship) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		ship[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_desert()
{
	const int CSVrow = 3;
	const int CSVcol = 0;
	const int filenum = sizeof(desert) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		desert[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_green()
{
	const int CSVrow = 4;
	const int CSVcol = 0;
	const int filenum = sizeof(green) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		green[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_vocano()
{
	const int CSVrow = 5;
	const int CSVcol = 0;
	const int filenum = sizeof(volcano) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		volcano[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::load_obstacle()
{
	const int CSVrow = 6;
	const int CSVcol = 0;
	const int filenum = sizeof(obstacle) / sizeof(Image*);
	char path[filenum][100];
	TCHAR Tpath[filenum][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		obstacle[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}
