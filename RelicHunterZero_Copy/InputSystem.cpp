#include "InputSystem.h"
#include "framework.h"

void Inputsystem::update()
{
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 45) return;
	oldTime = newTime;

	if (GetAsyncKeyState('W') & 0x8000)
		key_W = true;
		
	if(GetAsyncKeyState('A') & 0x8000)
		key_A = true;
		
	if(GetAsyncKeyState('S') & 0x8000)
		key_S = true;
		
	if(GetAsyncKeyState('D') & 0x8000)
		key_D = true;
		
	if(GetAsyncKeyState('Q') & 0x8000)
		key_Q = true;

	if (GetAsyncKeyState('E') & 0x8000)
		key_E = true;

	if(GetAsyncKeyState('R') & 0x8000)
		key_R = true;

	if(GetAsyncKeyState('F') & 0x8000)
		key_F = true;
		
	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		key_SHIFT = true;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		key_CTRL = true;

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		mou_R = true;
		
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		mou_L = true;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		mou_Wheel = true;

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		ESC = true;
}

void Inputsystem::reset()
{
	key_W = false;
	key_A = false;
	key_S = false;
	key_D = false;
	key_Q = false;
	key_E = false;
	key_R = false;
	key_F = false;
	key_SHIFT = false;
	key_CTRL = false;
	mou_R = false;
	mou_L = false;
	mou_Wheel = false;
	ESC = false;
}
