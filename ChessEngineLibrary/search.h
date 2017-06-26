#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <time.h>

#include "piece.h"
#include "moveGeneration.h"
#include "board.h"
#include "scoring.h"
#include "move.h"
#include "utils.h"
#include "transpositionTable.h"
#include "transpositionEntry.h"
#include "moveOrdering.h"

struct searchData
{
	int nodes;
	int depth;
	time_t startTime;
};

Move startSearch(int searchDepth, Board* board, TranspositionEntry* transpositionTable);
Move rootSearch(int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable);
int negamax(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet, TranspositionEntry* transpositionTable);
int quiescence(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet);
