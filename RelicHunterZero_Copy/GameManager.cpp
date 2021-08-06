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
	shutdown_image_func(menu);
	shutdown_image_func(maptoggle);
	shutdown_image_func(ship);
	shutdown_image_func(green);
	shutdown_image_func(desert);
	shutdown_image_func(volcano);
	shutdown_image_func(obstacle);
	shutdown_image_func(obstacle_cage);
	shutdown_image_func(item);
	shutdown_image_func(duck1_idle);
	shutdown_image_func(duck2_idle);
	shutdown_image_func(duck3_idle);
	shutdown_image_func(kamikaze1_idle);
	shutdown_image_func(kamikaze2_idle);
	shutdown_image_func(turtle1_idle);
	shutdown_image_func(turtle2_idle);
	shutdown_image_func(turtle3_idle);
	shutdown_image_func(boss_idle);





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
	load_obstacle_cage();
	load_item();
	load_enemy_idle();
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
	load_image_func(2, 0, sizeof(ship) / sizeof(Image*), ship);
}

void GameManager::load_desert()
{
	load_image_func(3, 0, sizeof(desert) / sizeof(Image*), desert);
}

void GameManager::load_green()
{
	load_image_func(4, 0, sizeof(green) / sizeof(Image*), green);
}

void GameManager::load_vocano()
{
	load_image_func(5, 0, sizeof(volcano) / sizeof(Image*), volcano);
}

void GameManager::load_obstacle()
{
	load_image_func(6, 0, sizeof(obstacle) / sizeof(Image*), obstacle);
}

void GameManager::load_obstacle_cage()
{
	load_image_func(7, 0, sizeof(obstacle_cage) / sizeof(Image*), obstacle_cage);
	
}

void GameManager::load_item()
{
	load_image_func(8, 0, sizeof(item) / sizeof(Image*), item);
}

void GameManager::load_enemy_idle()
{
	int a = 9;
	load_image_func(a++, 0, sizeof(duck1_idle) / sizeof(Image*), duck1_idle);
	load_image_func(a++, 0, sizeof(duck2_idle) / sizeof(Image*), duck2_idle);
	load_image_func(a++, 0, sizeof(duck3_idle) / sizeof(Image*), duck3_idle);
	load_image_func(a++, 0, sizeof(kamikaze1_idle) / sizeof(Image*), kamikaze1_idle);
	load_image_func(a++, 0, sizeof(kamikaze2_idle) / sizeof(Image*), kamikaze2_idle);
	load_image_func(a++, 0, sizeof(turtle1_idle) / sizeof(Image*), turtle1_idle);
	load_image_func(a++, 0, sizeof(turtle2_idle) / sizeof(Image*), turtle2_idle);
	load_image_func(a++, 0, sizeof(turtle3_idle) / sizeof(Image*), turtle3_idle);
	load_image_func(a++, 0, sizeof(boss_idle) / sizeof(Image*), boss_idle);
}

void GameManager::load_image_func(const int CSVrow, const int CSVcol, const int filenum, Image **img)
{
	char path[30][100];
	TCHAR Tpath[30][100];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		img[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::shutdown_image_func(Image ** img)
{
	for (int i = 0; i < sizeof(img) / sizeof(Image *); i++)
	{
		if (img[i])
			delete img[i];
	}
}
