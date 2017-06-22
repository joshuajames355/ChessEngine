#pragma once
#include <cstdlib> 
#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "transpositionTable.h"

#define TTSize 16777216

struct TranspositionEntry;

struct ZorbistKeys
{
	//A key for each square. WP = 0 
public:
	static uint64_t pieceKeys[64][12];
	static uint64_t enPassantKeys[8];
	static uint64_t blackMoveKey;
	static uint64_t blackQueenSideCastlingKey;
	static uint64_t blackKingSideCastlingKey;
	static uint64_t whiteQueenSideCastlingKey;
	static uint64_t whiteKingSideCastlingKey;
	
	static void initialize();
};

uint64_t get64rand();





