#pragma once
#include <cstdlib> 
#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "piece.h"
#include "board.h"
#include "move.h"

enum nodeType { Exact, lowerBound, upperBound };

struct TranspositionEntry
{
public:
	int zorbistKey;
	Move bestMove;
	nodeType flag;
	int score;
	int depth;
};

struct ZorbistKeys
{
	//A key for each square. WP = 0 
public:
	static int pieceKeys[64][12]; 
	static int blackMoveKey;
	static std::unordered_map<int, TranspositionEntry> TranspositionTable;
	static void initialize();
};


uint64_t get64rand();
int getZorbistKey(Board* board, colours colour);
int updateHash(Move newMove, int oldHash, colours colour);





