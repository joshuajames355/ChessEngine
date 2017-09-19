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
#include "timeManagement.h"
#include "transpositionTable.h"
#include "transpositionEntry.h"
#include "moveOrdering.h"

struct searchData
{
	int nodes;
	int depth;
	time_t startTime;
};

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer);
Move rootSearch(int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable);
int negascout(int alpha, int beta, int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable,bool hasBeenNullMove, bool isPrevMoveNull);
int quiescence(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet);
bool continueQuiescence(Board* board, Move* nextMove);
bool canDoNullSearch(Board* board);