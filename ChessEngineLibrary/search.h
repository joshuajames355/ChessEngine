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

struct PVData
{
	Move bestMove;
	std::string line;
};

void updateUI(searchData * data, Move currentMove, int currentMoveNumber, std::string pvLine, float alpha);
void finalUIUpdate(searchData * data, std::string pvLine);

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer);
int negascout(int alpha, int beta, int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable, std::array<std::array<std::array<Move, 64>, 64>, 2>* counterMoves, Move* prevMove, std::array<std::array<std::array<int, 64>, 64>, 2>* historyMoves);
int quiescence(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet);
bool continueQuiescence(Board* board, Move* nextMove);

PVData extractPVLine(Board* board, TranspositionEntry* transpositionTable, int expectedDepth);