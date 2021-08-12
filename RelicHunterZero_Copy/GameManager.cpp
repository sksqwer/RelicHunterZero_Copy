#include "GameManager.h"
#include "ScreenManager.h"
#include "FileSystem.h"
#include "GameScreen.h"

void GameManager::Init()
{
	GDI_Init();
	Load_Image();

	cursor[0] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur32.cur");
	cursor[1] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur42.cur");
	cursor[2] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur52.cur");
	cursor[3] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur62.cur");
	cursor[4] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur72.cur");
	cursor[5] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur82.cur");
	cursor[6] = Image::FromFile(L"Resource/Sprite/Crooshair/Original/Black_cur92.cur");
}

void GameManager::ShutDown()
{
	shutdown_image_func(cursor);
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
	shutdown_image_func(bigwindow);
	shutdown_image_func(etc);
	shutdown_image_func(bigwindow_reflection);
	shutdown_image_func(holo_Jimmy);
	shutdown_image_func(holo_Pinky);
	shutdown_image_func(holo_Biu);
	shutdown_image_func(holo_Punny);
	shutdown_image_func(holo_Raff);
	shutdown_image_func(Keytutorial);
	shutdown_image_func(NPC_ass);
	shutdown_image_func(Jimmy_idle);
	shutdown_image_func(Jimmy_walk);
	shutdown_image_func(Jimmy_dash);
	shutdown_image_func(Jimmy_sprint);
	shutdown_image_func(Jimmy_hit);
	shutdown_image_func(Jimmy_death);
	shutdown_image_func(Jimmy_teleportation_in);
	shutdown_image_func(Jimmy_teleportation_out);
	shutdown_image_func(effect_dash);
	shutdown_image_func(bullet);


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
	/*DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 500) return;
	oldTime = newTime;*/

	GetCursorPos(&mouse);

	Inputsystem::getInstance().update();


	if (Screen_flag == 7)
	{
		GameScreen::getInstance().update();
	}
}

void GameManager::show()
{
	
//	DWORD newTime = GetTickCount();
//	static DWORD oldTime = newTime;
//	if (newTime - oldTime < 30) return;
//	oldTime = newTime;


	ScreenManager::getInstance().Screen();
}

void GameManager::show_FPS(HDC hdc, int a, int b)
{

	TCHAR FPS[100];
	DWORD num;
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (oldTime != newTime)
		num = DWORD(1000) / (newTime - oldTime);
	else
		num = DWORD(144);
	_stprintf_s(FPS, _T("F P S : %lu"), num);


	ScreenManager::getInstance().DoubleBufferingtext(hdc, FPS, 1 + a, 1 + b, 100, 20, 12, Color(255, 255, 0, 255),2);
	oldTime = GetTickCount();
}

void GameManager::Load_Image()
{

	int a = 0;
	load_image_func(a++, 0, sizeof(menu) / sizeof(Image*), menu);
	load_image_func(a++, 0, sizeof(maptoggle) / sizeof(Image*), maptoggle);
	load_image_func(a++, 0, sizeof(ship) / sizeof(Image*), ship);
	load_image_func(a++, 0, sizeof(desert) / sizeof(Image*), desert); 
	load_image_func(a++, 0, sizeof(green) / sizeof(Image*), green);
	load_image_func(a++, 0, sizeof(volcano) / sizeof(Image*), volcano);
	load_image_func(a++, 0, sizeof(obstacle) / sizeof(Image*), obstacle);
	load_image_func(a++, 0, sizeof(obstacle_cage) / sizeof(Image*), obstacle_cage);
	load_image_func(a++, 0, sizeof(item) / sizeof(Image*), item);
	load_image_func(a++, 0, sizeof(duck1_idle) / sizeof(Image*), duck1_idle);
	load_image_func(a++, 0, sizeof(duck2_idle) / sizeof(Image*), duck2_idle);
	load_image_func(a++, 0, sizeof(duck3_idle) / sizeof(Image*), duck3_idle);
	load_image_func(a++, 0, sizeof(kamikaze1_idle) / sizeof(Image*), kamikaze1_idle);
	load_image_func(a++, 0, sizeof(kamikaze2_idle) / sizeof(Image*), kamikaze2_idle);
	load_image_func(a++, 0, sizeof(turtle1_idle) / sizeof(Image*), turtle1_idle);
	load_image_func(a++, 0, sizeof(turtle2_idle) / sizeof(Image*), turtle2_idle);
	load_image_func(a++, 0, sizeof(turtle3_idle) / sizeof(Image*), turtle3_idle);
	load_image_func(a++, 0, sizeof(boss_idle) / sizeof(Image*), boss_idle);
	load_image_func(a++, 0, sizeof(char_sel_toggle) / sizeof(Image*), char_sel_toggle);
	load_image_func(a++, 0, sizeof(char_sel_unselected) / sizeof(Image*), char_sel_unselected);
	load_image_func(a++, 0, sizeof(char_sel_stat) / sizeof(Image*), char_sel_stat);
	load_image_func(a++, 0, sizeof(char_sel_jimmy) / sizeof(Image*), char_sel_jimmy);
	load_image_func(a++, 0, sizeof(char_sel_jimmy_sel) / sizeof(Image*), char_sel_jimmy_sel);
	load_image_func(a++, 0, sizeof(bigwindow) / sizeof(Image*), bigwindow);
	load_image_func(a++, 0, sizeof(etc) / sizeof(Image*), etc);
	load_image_func(a++, 0, sizeof(bigwindow_reflection) / sizeof(Image*), bigwindow_reflection);
	load_image_func(a++, 0, sizeof(holo_Jimmy) / sizeof(Image*), holo_Jimmy);
	load_image_func(a++, 0, sizeof(holo_Pinky) / sizeof(Image*), holo_Pinky);
	load_image_func(a++, 0, sizeof(holo_Biu) / sizeof(Image*), holo_Biu);
	load_image_func(a++, 0, sizeof(holo_Punny) / sizeof(Image*), holo_Punny);
	load_image_func(a++, 0, sizeof(holo_Raff) / sizeof(Image*), holo_Raff);
	load_image_func(a++, 0, sizeof(Keytutorial) / sizeof(Image*), Keytutorial);
	load_image_func(a++, 0, sizeof(NPC_ass) / sizeof(Image*), NPC_ass);
	load_image_func(a++, 0, sizeof(Jimmy_idle) / sizeof(Image*), Jimmy_idle);
	load_image_func(a++, 0, sizeof(Jimmy_walk) / sizeof(Image*), Jimmy_walk);
	load_image_func(a++, 0, sizeof(Jimmy_dash) / sizeof(Image*), Jimmy_dash);
	load_image_func(a++, 0, sizeof(Jimmy_sprint) / sizeof(Image*), Jimmy_sprint);
	load_image_func(a++, 0, sizeof(Jimmy_hit) / sizeof(Image*), Jimmy_hit);
	load_image_func(a++, 0, sizeof(Jimmy_death) / sizeof(Image*), Jimmy_death);
	load_image_func(a++, 0, sizeof(Jimmy_teleportation_in) / sizeof(Image*), Jimmy_teleportation_in);
	load_image_func(a++, 0, sizeof(Jimmy_teleportation_out) / sizeof(Image*), Jimmy_teleportation_out);
	load_image_func(a++, 0, sizeof(effect_dash) / sizeof(Image*), effect_dash);
	load_image_func(a++, 0, sizeof(bullet) / sizeof(Image*), bullet);

}

void GameManager::load_image_func(const int CSVrow, const int CSVcol, const int filenum, Image **img)
{
	char path[100][200];
	TCHAR Tpath[100][200];

	for (int i = 0; i < filenum; i++)
	{
		Filesystem::getInstance().getpath(CSVrow, CSVcol, path, filenum);
		int msglen = MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, path[i], strlen(path[i]), Tpath[i], msglen);
		Tpath[i][msglen] = NULL;

		img[i] = Gdiplus::Image::FromFile(Tpath[i]);
	}
}

void GameManager::shutdown_image_func(Image **img)
{
	for (int i = 0; i < sizeof(img) / sizeof(Image *); i++)
	{
		if (img[i])
			delete img[i];
	}
}

Image * GameManager::returnimagepointer(int n, int m)
{
	int a = 0;
	switch (n)
	{
	case 0:
		return (GameManager::getInstance().menu)[m];
	case 1:
		return (GameManager::getInstance().maptoggle)[m];
	case 2:
		return (GameManager::getInstance().ship)[m];
	case 3:
		return (GameManager::getInstance().desert)[m];
	case 4:
		return (GameManager::getInstance().green)[m];
	case 5:
		return (GameManager::getInstance().volcano)[m];
	case 6:
		return (GameManager::getInstance().obstacle)[m];
	case 7:
		return (GameManager::getInstance().obstacle_cage)[m];
	case 8:
		return (GameManager::getInstance().item)[m];
	case 9:
		return (GameManager::getInstance().duck1_idle)[m];
	case 10:
		return (GameManager::getInstance().duck2_idle)[m];
	case 11:
		return (GameManager::getInstance().duck3_idle)[m];
	case 12:
		return (GameManager::getInstance().kamikaze1_idle)[m];
	case 13:
		return (GameManager::getInstance().kamikaze2_idle)[m];
	case 14:
		return (GameManager::getInstance().turtle1_idle)[m];
	case 15:
		return (GameManager::getInstance().turtle2_idle)[m];
	case 16:
		return (GameManager::getInstance().turtle3_idle)[m];
	case 17:
		return (GameManager::getInstance().boss_idle)[m];
	case 18:
		return (GameManager::getInstance().char_sel_toggle)[m];
	case 19:
		return (GameManager::getInstance().char_sel_unselected)[m];
	case 20:
		return (GameManager::getInstance().char_sel_stat)[m];
	case 21:
		return (GameManager::getInstance().char_sel_jimmy)[m];
	case 22:
		return (GameManager::getInstance().char_sel_jimmy_sel)[m];
	case 23:
		return (GameManager::getInstance().bigwindow)[m];
	case 24:
		return (GameManager::getInstance().etc)[m];
	case 25:
		return (GameManager::getInstance().bigwindow_reflection)[m];
	case 26:
		return (GameManager::getInstance().holo_Jimmy)[m];
	case 27:
		return (GameManager::getInstance().holo_Pinky)[m];
	case 28:
		return (GameManager::getInstance().holo_Biu)[m];
	case 29:
		return (GameManager::getInstance().holo_Punny)[m];
	case 30:
		return (GameManager::getInstance().holo_Raff)[m];
	case 31:
		return (GameManager::getInstance().Keytutorial)[m];
	case 32:
		return (GameManager::getInstance().NPC_ass)[m];
	case 33:
		return (GameManager::getInstance().Jimmy_idle)[m];
	case 34:
		return (GameManager::getInstance().Jimmy_walk)[m];
	case 35:
		return (GameManager::getInstance().Jimmy_dash)[m];
	case 36:
		return (GameManager::getInstance().Jimmy_sprint)[m];
	case 37:
		return (GameManager::getInstance().Jimmy_hit)[m];
	case 38:
		return (GameManager::getInstance().Jimmy_death)[m];
	case 39:
		return (GameManager::getInstance().Jimmy_teleportation_in)[m];
	case 40:
		return (GameManager::getInstance().Jimmy_teleportation_out)[m];
	case 41:
		return (GameManager::getInstance().effect_dash)[m];
	case 42:
		return (GameManager::getInstance().bullet)[m];



	}
	return nullptr;
}
