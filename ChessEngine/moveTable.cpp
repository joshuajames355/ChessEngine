#include "stdafx.h"
#include "moveTable.h"

moveTable::moveTable()
{

}

moveTable::moveTable(std::string filename, pieceType typeNew, bool mustFlip)
{
	loadFromFile(filename);
	type = typeNew;

}

void moveTable::loadFromFile(std::string filename)
{
	std::ifstream tableFile;
	tableFile.open(filename);
	std::string line;
	int lineNum = 0;

	if (tableFile.is_open())
	{
		while (std::getline(tableFile, line))
		{
			std::string temp = "";
			std::array<int,2> movesTemp;
			int counter2 = 0;
			for (int counter = 0; counter < line.size(); counter++)
			{
				if (line[counter] != ',')
				{
					temp += line[counter];
				}
				else
				{
					movesTemp[counter2] = std::stoi(temp);
					counter2++;
					temp = "";
				}
			}
			moves.push_back(movesTemp);
			lineNum++;
		}
		tableFile.close();
	}

	std::cout << "\nMove Table\n\n";
	for (int x = 0; x < moves.size(); x++)
	{

		std::cout << moves[x][0] << " " << moves[x][1];
	}
	std::cout << "\n\n";
}

