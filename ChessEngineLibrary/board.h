#pragma once
#include <stdint.h>
#include <cmath>
#include <iostream>
#include "piece.h"

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

	uint64_t findBitboard(colours colour , pieceType piece);
	void setBitboard(colours colour, pieceType piece, uint64_t bitboard);
	void removePiece(uint64_t bitboard);

	uint64_t whitePawnBitboard;
	uint64_t whiteKnightBitboard;
	uint64_t whiteBishopBitboard;
	uint64_t whiteRookBitboard;
	uint64_t whiteQueenBitboard;
	uint64_t whiteKingBitboard;
	
	uint64_t blackPawnBitboard;
	uint64_t blackKnightBitboard;
	uint64_t blackBishopBitboard;
	uint64_t blackRookBitboard;
	uint64_t blackQueenBitboard;
	uint64_t blackKingBitboard;

	uint64_t whitePieces;
	uint64_t blackPieces;
	uint64_t allPieces;
};

int bitSum(uint64_t bitboard);
uint64_t pop(uint64_t& bitboard);