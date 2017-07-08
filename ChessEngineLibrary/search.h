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

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer, std::unordered_map<uint64_t, std::string>* openingBook);
Move rootSearch(int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable);
int negascout(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable);
int quiescence(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet);
