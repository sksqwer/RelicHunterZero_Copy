#pragma once
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

class Filesystem
{
private:
	const char *csvpath = "Resource/ResourceLog.csv";
public:
	void getpath(int i, char path[][100], int n);
	void csv_read_row(char *ch, std::istream &infile, char delimiter);


};

#endif _FILESYSTEM_H_