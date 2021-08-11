#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "framework.h"
#include "gun.h"
#include "bullet.h"

class Player
{
public:
	enum {IDLE, WALK, DASH, SPRINT, HIT, DEATH, TELEPOTATION_IN, TELEPORTATION_OUT};
	POINT worldpos = { 1200, 1100 };
	POINT mappos = { 960, 590 };
	int maxhp = 100;
	int maxshield = 40;
	int maxstamina = 100;
	int HP = 100;
	int shield = 40;
	int Stamina = 100;
	int state = 0;
	int velocity = 10;
	int maxframe = 12;
	int col = 33;
	int row = 0;
	//

	Gun *curgun;
	Gun *gun1, *gun2;
	std::vector<Bullet *> bullet_pool;
	std::vector<Bullet *> bullet_using;
	int bullet_pool_maxsize = 100;
	int cur = 0;
	//function
	void init();
	void shutdown();
	void setstate();
	void update();
	void shot_bullet(POINTF, POINT, POINT);
	//





};


#endif // !_PLAYER_H_
