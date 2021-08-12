#pragma once
#ifndef _GUN_H_
#define _GUN_H_

class Gun
{
public:
	enum { PISTOL, SMG, SHOTGUN , RIFLE , SNIPER , MACHINEGUN };
	enum {IDLE = 100, SHOOT = 101};
	int kind = PISTOL;
	int state = IDLE;
	int impact = 10;
	int range = 800;
	int rate = 300;
	int bullet_speed = 100;
	int maxbullet = 30;
	int curbullet = 30;
	int bullet_cost = 1;
	int bullet_type = 0; //LIGHT, NORMAL, HEAVY, SNIPER, ENEMY
	//image
	int col = 8;
	int row = 0;
	//
	void set_gun();


};

#endif // !_GUN_H_
