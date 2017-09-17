#pragma once
#include "pieceSquare.h"
#include "board.h"

int calculateScoreDiff(Board* board);

int calculatePawnStructureScore(Board* board);
int calculateRookPositionScore(Board* board);
int calculateMaterialScore(Board * board, bool lateGame);
int calculateKingSafetyScore(Board* board);
int calculateKingSafetyScoreForColour(Board * board, colours colour);

bool isLateGame(Board* board);