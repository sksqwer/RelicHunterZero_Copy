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
	GDI_ShutDown();

	delete[] menu;
	delete[] maptoggle;
	delete[] ship;
	delete[] green;
	delete[] desert;
	delete[] vocano;
	delete[] obstacle;
//	delete screenmanager;
//	delete filemanager;
//	delete ScM;
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
	/*
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 40) return;
	oldTime = newTime;*/


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

void GameManager::load_green()
{
}

void GameManager::load_desert()
{
}

void GameManager::load_vocano()
{
}

void GameManager::load_obstacle()
{
}
