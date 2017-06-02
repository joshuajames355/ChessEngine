#pragma once
#include <stdint.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "piece.h"

#define emptyBitboard 0
#define universalBitboard 18446744073709551615 //2**64 - 1

#define rank1 255
#define rank2 65280
#define rank3 16711680
#define rank4 4278190080
#define rank5 1095216660480
#define rank6 280375465082880
#define rank7 71776119061217280
#define rank8 18374686479671623680

#define fileA 72340172838076673
#define fileB 144680345676153346
#define fileC 289360691352306692
#define fileD 578721382704613384
#define fileE 1157442765409226768
#define fileF 2314885530818453536
#define fileG 4629771061636907072
#define fileH 9259542123273814144

class Board
{
public:
	Board();
	void defaults();
	void printBoard();
	void update();
	colours loadFromFen(std::string fen);

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
int bitScanForward(uint64_t bitboard);
std::vector<int> getSetBits(uint64_t bitboard);