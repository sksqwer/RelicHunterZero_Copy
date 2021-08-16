#include "Item_info.h"

item_ingame& item_ingame::operator=(const item_info & mapobj)
{
	this->in_use = mapobj.in_use;
	this->is_ani = mapobj.is_ani;
	this->max_frame = mapobj.max_frame;
	this->on_obj = mapobj.on_obj; // 0 = empty, 1 = OBSTACLE, 2 = ITEM, 3 = enemy
	this->pos = mapobj.pos;
	this->col = mapobj.col;
	this->row = mapobj.row;
	this->object_type = mapobj.object_type;
	this->attr = mapobj.attr;
	this->rotation = mapobj.rotation;
	//item
	this->pos = mapobj.pos;
	this->kind = mapobj.kind;
	return *this;
}

void item_ingame::animation_set()
{
	switch(kind)
	{
	case GRENADE:
		is_ani = true;
		row = 0;
		col = 47;
		max_frame = 17;
		break;
	case HEAL:
		is_ani = true;
		row = 0;
		col = 48;
		max_frame = 2;
		break;
	case BIGHEAL:
		is_ani = true;
		row = 0;
		col = 49;
		max_frame = 2;
		break;
	case HEAVYAMMO:
		is_ani = true;
		row = 0;
		col = 50;
		max_frame = 16;
		break;
	case MEDIUMAMMO:
		is_ani = true;
		row = 0;
		col = 51;
		max_frame = 16;
		break;
	case LIGHTAMMO:
		is_ani = true;
		row = 0;
		col = 52;
		max_frame = 16;
		break;
	case SHIELD:
		is_ani = true;
		row = 0;
		col = 53;
		max_frame = 2;
		break;
	}
}
