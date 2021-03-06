#include "transpositionTable.h"


void ZorbistKeys::initialize()
{
	srand(25461);

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			pieceKeys[i][j] = get64rand();
		}
	}
	for (int i = 0; i < 8; i++)
	{
		enPassantKeys[i] = get64rand();
	}

	blackMoveKey = get64rand();
	blackQueenSideCastlingKey = get64rand();
	blackKingSideCastlingKey = get64rand();
	whiteQueenSideCastlingKey = get64rand();
	whiteKingSideCastlingKey = get64rand();
}

uint64_t get64rand() {
	bool isCollision = true;
	uint64_t num;

	while (isCollision)
	{
		num = (((uint64_t)rand() << 0) & 0x000000000000FFFFull) |
			(((uint64_t)rand() << 16) & 0x00000000FFFF0000ull) |
			(((uint64_t)rand() << 32) & 0x0000FFFF00000000ull) |
			(((uint64_t)rand() << 48) & 0xFFFF000000000000ull);

		isCollision = false;
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if (num == ZorbistKeys::pieceKeys[i][j]) isCollision = true;
			}
		}
		if (num == 0) isCollision = true;
		if (num == ZorbistKeys::blackMoveKey) isCollision = true;
		if (num == ZorbistKeys::blackQueenSideCastlingKey) isCollision = true;
		if (num == ZorbistKeys::blackKingSideCastlingKey) isCollision = true;
		if (num == ZorbistKeys::whiteQueenSideCastlingKey) isCollision = true;
		if (num == ZorbistKeys::whiteKingSideCastlingKey) isCollision = true;
	}

	return num;
}

uint64_t ZorbistKeys::pieceKeys[64][12];
uint64_t ZorbistKeys::blackMoveKey;
uint64_t ZorbistKeys::blackQueenSideCastlingKey;
uint64_t ZorbistKeys::blackKingSideCastlingKey;
uint64_t ZorbistKeys::whiteQueenSideCastlingKey;
uint64_t ZorbistKeys::whiteKingSideCastlingKey;
uint64_t ZorbistKeys::enPassantKeys[8];