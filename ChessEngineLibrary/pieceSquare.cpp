#include "pieceSquare.h"


pieceSquare::pieceSquare()
{
}

pieceSquare::pieceSquare(std::string filename, pieceType typeNew, colours defaultColourNew)
{
	loadFromFile(filename);
	defaultColour = defaultColourNew;
	type = typeNew;
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
	int bitPos, x, y;
	while (bitboard)
	{
		bitPos = bitScanForward(pop(bitboard));
		if (targetColour == defaultColour)
		{
			bitPos = 63 - bitPos;
		}
		x = bitPos % 8;
		y = (bitPos - x) / 8;
		score += square[x][y];
	}
	return score;
}

pieceSquare pieceSquareData::pawnSquare = pieceSquare("WPSquareTable.txt", pawn, white);
pieceSquare pieceSquareData::knightSquare = pieceSquare("WNSquareTable.txt", knight, white);
pieceSquare pieceSquareData::bishopSquare = pieceSquare("WBSquareTable.txt", bishop, white);
pieceSquare pieceSquareData::rookSquare = pieceSquare("WRSquareTable.txt", rook, white);
pieceSquare pieceSquareData::queenSquare = pieceSquare("WQSquareTable.txt", queen, white);
pieceSquare pieceSquareData::midGameKingSquare = pieceSquare("WKMiddleSquareTable.txt", king, white);
pieceSquare pieceSquareData::lateGameKingSquare = pieceSquare("WKEndSquareTable.txt", king, white);

