#include "Record.h"

void Record::get_record()
{
	char Filename[100] = "Resourc/Record/Jimmy.txt";

	std::ifstream fin;
	fin.open(Filename, std::ios::binary);

	fin.seekg(0, std::ios::end);
	int size = fin.tellg();
	fin.seekg(0, std::ios::beg);


	if (size > 0)
	{
		fin.read((char *)Play_Time, sizeof(DWORD));
		fin.read((char *)Kill, sizeof(DWORD));
		fin.read((char *)Death, sizeof(DWORD));
		fin.read((char *)Boss_Kill, sizeof(DWORD));
		fin.read((char *)Best_Run_Time, sizeof(DWORD));
		fin.read((char *)Longest_Run_Time, sizeof(DWORD));
	}


	fin.close();

}

void Record::set_record()
{
	char Filename[100] = "Resourc/Record/Jimmy.txt";

	std::ofstream fout;
	fout.open(Filename, std::ios::binary);

	fout.write((char *)Play_Time, sizeof(DWORD));
	fout.write((char *)Kill, sizeof(DWORD));
	fout.write((char *)Death, sizeof(DWORD));
	fout.write((char *)Boss_Kill, sizeof(DWORD));
	fout.write((char *)Best_Run_Time, sizeof(DWORD));
	fout.write((char *)Longest_Run_Time, sizeof(DWORD));

	fout.close();
}
