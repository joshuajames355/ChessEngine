#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <array>
#include <stdint.h>

#include "piece.h"


class pieceSquare
{
public:
	pieceSquare();
	pieceSquare(std::string filename , pieceType typeNew , colours defaultColourNew);
	void loadFromFile(std::string filename);

	int calcScore(int64_t bitboard, colours targetColour);

	int square[8][8];
	colours defaultColour;
	pieceType type;
};

struct pieceSquareData
{
	static pieceSquare pawnSquare, knightSquare, bishopSquare, rookSquare, queenSquare, midGameKingSquare, lateGameKingSquare;
};
