#pragma once
#include "move.h"

enum nodeType { Exact, lowerBound, upperBound };

struct TranspositionEntry
{
public:
	uint64_t zorbistKey;
	Move bestMove;
	nodeType flag;
	int score;
	int depth;
};
