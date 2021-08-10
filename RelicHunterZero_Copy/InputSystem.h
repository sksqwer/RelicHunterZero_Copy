#pragma once
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

class Inputsystem
{
public:
	bool key_W = false;
	bool key_A = false;
	bool key_S = false;
	bool key_D = false;
	bool key_Q = false;
	bool key_E = false;
	bool key_R = false;
	bool key_F = false;
	bool key_SHIFT = false;
	bool mou_R = false;
	bool mou_L = false;
	bool mou_Wheel = false;
	bool ESC = false;
	bool Space = false;

	//special
	bool key_F1 = false;
	bool key_F2 = false;
	bool key_CTRL = false;
	//singleton
	Inputsystem() {}
	Inputsystem(const Inputsystem& ref) {}
	Inputsystem& operator=(const Inputsystem& ref) {}
	~Inputsystem() {}
	static Inputsystem& getInstance() { static Inputsystem Is; return Is; }
	//singleton
	void reset();
	void update();



};

#endif _INPUT_SYSTEM_H_
