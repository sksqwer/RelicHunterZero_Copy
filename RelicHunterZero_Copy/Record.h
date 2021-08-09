#pragma once
#ifndef _RECORD_H_
#define _RECORD_H_

#include "framework.h"

class Record
{
public:
	DWORD Play_Time = 0;
	DWORD Kill = 0;
	DWORD Death = 0;
	DWORD Boss_Kill = 0;
	DWORD Best_Run_Time = 0;
	DWORD Longest_Run_Time = 0;
	//
	DWORD oldTime;

	//singleton
	Record() {}
	Record(const Record& ref) {}
	Record& operator=(const Record& ref) {}
	~Record() {}
	static Record& getInstance() { static Record SM; return SM; }

	//
	void get_record();
	void set_record();


};


#endif // !_RECORD_H_
