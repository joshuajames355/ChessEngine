#pragma once
#include <stdint.h>
#include <cmath>
#include <iostream>

#define emptyBitboard 0
#define universalBitboard 18446744073709551615 //2**64 - 1

#define rank0 255
#define rank1 65280
#define rank2 16711680
#define rank3 4278190080
#define rank4 1095216660480
#define rank5 280375465082880
#define rank6 71776119061217280
#define rank7 18374686479671623680

#define fileA 9259542123273814144
#define fileB 4629771061636907072
#define fileC 2314885530818453536
#define fileD 1157442765409226768
#define fileE 578721382704613384
#define fileF 289360691352306692
#define fileG 144680345676153346
#define fileH 72340172838076673

class Board
{
public:
	Board();
	void defaults();
	void printBoard();
	void update();

	int64_t whitePawnBitboard;
	int64_t whiteKnightBitboard;
	int64_t whiteBishopBitboard;
	int64_t whiteRookBitboard;
	int64_t whiteQueenBitboard;
	int64_t whiteKingBitboard;
	
	int64_t blackPawnBitboard;
	int64_t blackKnightBitboard;
	int64_t blackBishopBitboard;
	int64_t blackRookBitboard;
	int64_t blackQueenBitboard;
	int64_t blackKingBitboard;

	int64_t whitePieces;
	int64_t blackPieces;
	int64_t allPieces;
};

int bitSum(int64_t bitboard);
int64_t pop(int64_t& bitboard);