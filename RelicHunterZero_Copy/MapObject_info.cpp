#include "MapObject_info.h"

MapObject_ingame& MapObject_ingame::operator=(const MapObject_info & mapobj)
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
	return *this;
}
