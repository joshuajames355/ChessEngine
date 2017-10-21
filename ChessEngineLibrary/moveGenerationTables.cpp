#include "moveGenerationTables.h"
#include "bitboard.h"

void setupMoveGen()
{
	uint64_t currentPos, knightMoves, kingMoves;
	for (int x = 0; x < 64; x++)
	{
		currentPos = (uint64_t)1 << x;

		knightMoves = 0;
		knightMoves |= currentPos << 15 & ~fileH;
		knightMoves |= currentPos << 17 & ~fileA;
		knightMoves |= currentPos << 06 & ~fileH & ~fileG;
		knightMoves |= currentPos << 10 & ~fileA & ~fileB;
		knightMoves |= currentPos >> 10 & ~fileH & ~fileG;
		knightMoves |= currentPos >> 06 & ~fileA & ~fileB;
		knightMoves |= currentPos >> 17 & ~fileH;
		knightMoves |= currentPos >> 15 & ~fileA;
		knightMovesArray[x] = knightMoves;

		kingMoves = 0;
		kingMoves |= currentPos << 8;
		kingMoves |= currentPos << 9 & ~fileA;
		kingMoves |= currentPos << 1 & ~fileA;
		kingMoves |= currentPos >> 7 & ~fileA;
		kingMoves |= currentPos >> 8;
		kingMoves |= currentPos >> 9 & ~fileH;
		kingMoves |= currentPos >> 1 & ~fileH;
		kingMoves |= currentPos << 7 & ~fileH;
		kingMovesArray[x] = kingMoves;

		pawnWhiteAttacksArray[x] = currentPos << 7 & ~fileH | currentPos << 9 & ~fileA;
		pawnBlackAttacksArray[x] = currentPos >> 9 & ~fileH | currentPos >> 7 & ~fileA;
	}
}