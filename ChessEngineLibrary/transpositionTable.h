#pragma once
#include <cstdlib> 
#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "piece.h"
#include "board.h"
#include "move.h"

#define TTSize 16777216



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

struct ZorbistKeys
{
	//A key for each square. WP = 0 
public:
	static uint64_t pieceKeys[64][12];
	static uint64_t blackMoveKey;
	static TranspositionEntry TranspositionTable[TTSize];
	//static std::unordered_map<uint64_t, TranspositionEntry> TranspositionTable;
	
	static void initialize();
};


uint64_t get64rand();
uint64_t getZorbistKey(Board* board);
uint64_t updateHash(Move newMove, uint64_t oldHash, colours colour);





