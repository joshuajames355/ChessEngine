#pragma once
#include "pieceSquare.h"
#include "board.h"

int calculateScoreDiff(Board* board);
int calculateMaterialScore(Board* board);
int calculatePositionalScore(Board* board);