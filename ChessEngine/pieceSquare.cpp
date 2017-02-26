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

}

int pieceSquare::calcScore(std::array<std::array<piece, 8>, 8> board, colours targetColour)
{
	int score = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y].type== type && board[x][y].colour == targetColour)
			{
				if (targetColour == defaultColour) // Colour is correct
				{
					score += square[x][y];
				}
				else //Needs to flip the board to work with the other colour
				{
					score += square[x][7 - y];
				}
			}
		}
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

