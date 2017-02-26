#pragma once
#include <array>

#include "piece.h"
#include "pieceSquare.h"

class counter
{
public:
	counter(std::array<std::array<piece, 8>, 8> board);
	int WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK;
};

int calculateScoreDiff(std::array<std::array<piece, 8>, 8> board, colours aiColour);
int calculateMaterialScore(std::array<std::array<piece, 8>, 8> board, colours aiColour, counter pieceCounts);
int calculatePositionalScore(std::array<std::array<piece, 8>, 8> board, colours aiColour, counter pieceCounts);