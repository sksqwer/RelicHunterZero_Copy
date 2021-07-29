
#include "framework.h"
#include "FileSystem.h"

void Filesystem::getpath(int itr, char path[][100], int n)
{
	std::ifstream infile(csvpath);

	if (infile.fail())
	{
		MessageBox(NULL, _T("file reading failed"), _T("Error"), MB_OK);
		return;
	}
	int i = 0;
	char temp[100];

	while (i != itr && infile.good())
	{
		csv_read_row(temp, infile, ',');

		i++;
	}

	for (int i = 0; i < n; i++)
	{
		if (!infile.good()) break;
		csv_read_row(path[itr + i], infile, ',');
	}

	infile.close();

}

void Filesystem::csv_read_row(char *ch, std::istream & infile, char delimiter)
{
	int i = 0;

	while (infile.good())
	{
		char c = infile.get();

		if (c == delimiter)
			break;
		else if (c == '\n' || c == '\r')
			continue;


		ch[i++] = c;
		ch[i] = NULL;
	}


}
