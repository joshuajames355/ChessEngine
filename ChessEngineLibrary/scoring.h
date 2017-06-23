#pragma once
#include "pieceSquare.h"
#include "board.h"

int calculateScoreDiff(Board* board);
int calculatePositionalScore(Board* board);
bool isLateGame(Board* board);