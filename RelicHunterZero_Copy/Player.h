#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "framework.h"
class Gun;
class Bullet;
class Casing;

class Player
{
public:
	enum {IDLE, WALK, DASH, SPRINT, HIT, DEATH, TELEPOTATION_IN, TELEPORTATION_OUT};
	POINT worldpos = { 1200, 1100 };
	POINT mappos = { 960, 590 };
	int maxhp = 100;
	int maxshield = 40;
	int maxstamina = 100;
	int HP = 50;
	int shield = 20;
	int Stamina = 100;
	int state = IDLE;
	//ammo
	int light_ammo = 100;
	int ammo_max = 1000;
	int medium_ammo = 0;
	int heavy_ammo = 0;
	int grenade = 3;
	int grenade_max = 5;

	//
	int velocity = 10;
	int maxframe = 12;
	int col = 33;
	int row = 0;
	//

	Gun *curgun;
	Gun *gun1, *gun2;
	std::vector<Bullet *> bullet_pool;
	std::vector<Bullet *> bullet_using;
	std::vector<Casing *> casing_pool;
	std::vector<Casing *> casing_using;
	int bullet_pool_maxsize = 100;
	int cur = 0;
	//function
	void init();
	void shutdown();
	void setstate();
	void update();
	void shot_bullet(POINTF, POINT, POINT, int);
	void shot_casing(POINTF p, POINTF d, int pz, int mpz, int t);
	//





};


#endif // !_PLAYER_H_
