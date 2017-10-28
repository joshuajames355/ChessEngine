#include "pieceSquare.h"


pieceSquare::pieceSquare()
{
}

pieceSquare::pieceSquare(std::string filename, pieceType typeNew, colours defaultColourNew)
{
	defaultColour = defaultColourNew;
	type = typeNew;
	loadFromFile(filename);
}

void pieceSquare::loadFromFile(std::string filename)
{
	std::ifstream tableFile;
	tableFile.open(filename);
	std::string line;
	int lineNum = 0;

	if (tableFile.is_open())
	{
		while (std::getline(tableFile, line))
		{
			int colNum = 0;
			std::string temp = "";
			for (int counter = 0; counter < line.size(); counter++)
			{
				if (line[counter] != ',')
				{
					temp += line[counter];
				}
				else
				{
					square[colNum][lineNum] = std::stoi(temp);
					temp = "";
					colNum++;
				}
			}
			lineNum++;
		}
		tableFile.close();
	}
	else
	{
		std::cout << "Failed to find " + filename + "\n";
	}

}

int pieceSquare::calcScore(uint64_t bitboard,colours targetColour)
{

	int score = 0;
	int bitPos;
	while (bitboard)
	{
		bitPos = bitScanForward(pop(bitboard));
		score += getScoreFromPos(bitPos, targetColour);
	}
	return score;
}

int pieceSquare::getScoreFromPos(int pos, colours targetColour)
{
	if (targetColour == defaultColour)
	{
		pos = 63 - pos;
	}
	int x = pos % 8;
	int y = (pos - x) / 8;
	return square[x][y];
}

pieceSquare pieceSquareData::pawnSquare = pieceSquare("WPSquareTable.txt", pawn, white);
pieceSquare pieceSquareData::knightSquare = pieceSquare("WNSquareTable.txt", knight, white);
pieceSquare pieceSquareData::bishopSquare = pieceSquare("WBSquareTable.txt", bishop, white);
pieceSquare pieceSquareData::midGameKingSquare = pieceSquare("WKMiddleSquareTable.txt", king, white);
pieceSquare pieceSquareData::lateGameKingSquare = pieceSquare("WKEndSquareTable.txt", king, white);

