#pragma once
#include "pieceSquare.h"
#include "board.h"

int calculateScoreDiff(Board* board, colours aiColour);
int calculateMaterialScore(Board* board, colours aiColour);
int calculatePositionalScore(Board* board, colours aiColour);