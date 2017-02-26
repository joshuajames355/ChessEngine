#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <array>

#include "ChessEngine.h"
#include "piece.h"

class pieceSquare
{
public:
	pieceSquare();
	pieceSquare(std::string filename , pieceType typeNew , colours defaultColourNew);
	void loadFromFile(std::string filename);

	int calcScore(std::array<std::array<piece, 8>, 8> board, colours aiColour);

	int square[8][8];
	colours defaultColour;
	pieceType type;
};

struct pieceSquareData
{
	static pieceSquare pawnSquare, knightSquare, bishopSquare, rookSquare, queenSquare, midGameKingSquare, lateGameKingSquare;
};
