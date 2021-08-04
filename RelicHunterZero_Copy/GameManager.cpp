#include "GameManager.h"
#include "ScreenManager.h"
#include "FileSystem.h"

void GameManager::Init()
{
	GDI_Init();
//	ScM = new ScreenManger();
//	screenmanager = new ScreenManager();
//	filemanager = new Filesystem();
}

void GameManager::ShutDown()
{
	GDI_ShutDown();
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

void GameManager::show(HWND hWnd, HDC hdc)
{/*
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 40) return;
	oldTime = newTime;*/


	ScreenManager::getInstance().Screen(hWnd, hdc);
}
