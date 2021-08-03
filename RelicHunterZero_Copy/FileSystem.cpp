
#include "framework.h"
#include "FileSystem.h"

void Filesystem::getpath(int row, int col, char path[][100], int n)
{
	std::ifstream infile(csvpath);

	if (infile.fail())
	{
		MessageBox(NULL, _T("file reading failed"), _T("Error"), MB_OK);
		return;
	}
	int i = 0;
	char temp[100];

	while (i != row && infile.good())
	{
		csv_read_row(temp, infile, '\n');

		i++;
	}
	i = 0;
	while (i != col && infile.good())
	{
		csv_read_row(temp, infile, ',');

		i++;
	}
	for (int i = 0; i < n; i++)
	{
		if (!infile.good()) break;
		csv_read_row(path[i], infile, ',');
	}

	infile.close();

}

void Filesystem::csv_read_row(char *ch, std::istream & infile, char delimiter)
{
	int i = 0;

	while (infile.good())
	{
		char c = infile.get();

		if (delimiter == '\n' && c != delimiter)
			continue;

		if (c == delimiter || c == '\n')
			break;
		else if ( c == '\r')
			continue;


		ch[i++] = c;
		ch[i] = NULL;
	}


}
