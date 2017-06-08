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

struct searchData
{
	int nodes;
	int depth;
	time_t startTime;
};

Move startSearch(int searchDepth, Board board, colours colour);
Move rootSearch(int depthLeft, Board board, colours colour);
int negamax(int alpha, int beta, int depthLeft, Board board, colours colour, searchData* data, uint64_t hash, bool isQuiet);