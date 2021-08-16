#include "obstacle_info.h"

obstacle_ingame& obstacle_ingame::operator=(const obstacle_info & mapobj)
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
	//obstacle
	this->obstacle_type = mapobj.obstacle_type;
	this->kind = mapobj.kind;
	this->block = mapobj.block;
	this->framelimit = mapobj.framelimit;

	return *this;
}
