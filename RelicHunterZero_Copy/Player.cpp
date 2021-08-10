#include "Player.h"

#include "InputSystem.h"

void Player::setstate()
{
	switch (state)
	{
	case 0:
		col = 33;
		row = 0;
		maxframe = 12;
		break;
	case 1:
		col = 34;
		row = 0;
		maxframe = 6;
		break;
	case 2:
		col = 35;
		row = 0;
		maxframe = 2;
		break;
	case 3:
		col = 36;
		row = 0;
		maxframe = 6;
		break;
	case 4:
		col = 37;
		row = 0;
		maxframe = 2;
		break;
	case 5:
		col = 38;
		row = 0;
		maxframe = 11;
		break;
	case 6:
		col = 39;
		row = 0;
		maxframe = 8;
		break;
	case 7:
		col = 40;
		row = 0;
		maxframe = 8;
		break;
	}
}

void Player::update()
{
	row++;
	int pre = state;
	static int dash_count = 0;

	if (state == DASH)
	{
		dash_count++;
		if (dash_count < 6)
		{
			row %= maxframe;
			return;
		}
	}
	
	if (Inputsystem::getInstance().key_S || Inputsystem::getInstance().key_A || Inputsystem::getInstance().key_W || Inputsystem::getInstance().key_D)
	{
		state = WALK;
		velocity = 10;

		if (Inputsystem::getInstance().Space)
		{
			if (Stamina > 0)
			{
				state = SPRINT;
				velocity = 20;
				Stamina -= 2;
			}
		}
		else if (Inputsystem::getInstance().key_SHIFT)
		{
			DWORD newTime = GetTickCount();
			static DWORD dash_oldTime = newTime;
			if (newTime - dash_oldTime >= 1000)
			{
				if (Stamina >= 50)
				{
					dash_count = 0;
					state = DASH;
					velocity = 50;
					Stamina -= 50;
					dash_oldTime = newTime;
				}
			}

		}
		else
		{
			Stamina += 2;
			if (Stamina > 100)
				Stamina = 100;
		}
	}
	else
	{
		state = IDLE;
		velocity = 10;
	}



	row %= maxframe;
	if(pre != state)
		setstate();
}