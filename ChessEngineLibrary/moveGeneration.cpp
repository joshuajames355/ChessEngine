#include "moveGeneration.h"

std::vector<Board> searchForMoves(Board * board, colours aiColour)
{
	std::vector<Board> moveList;

	if (aiColour == white)
	{
		int64_t pawnMoves =  board->whitePawnBitboard << 8 & ~board->allPieces; // Moves forwards

		int64_t pawnAttacks = board->whitePawnBitboard << 7 & board->blackPieces & fileH | board->whitePawnBitboard << 9 & board->blackPieces & ~fileA; //Pieces that can be attacked
	}
	else
	{
		int64_t pawnMoves = board->whitePawnBitboard >> 8 & ~board->allPieces; // Moves forwards

		int64_t pawnAttacks = board->whitePawnBitboard >> 7 & board->blackPieces & fileA| board->whitePawnBitboard >> 9 & board->blackPieces & ~fileH; //Pieces that can be attacked
	}

	return moveList;
}

void generatePawnMoves(Board* board, colours aiColour, std::vector<Board>& Movelist)
{
	if (aiColour == white)
	{
		int64_t pawnBitboard = board->whitePawnBitboard;
		while (pawnBitboard)
		{
			int64_t pawnPos = pop(pawnBitboard);

			int64_t pawnMove = pawnPos << 8 & ~board->allPieces; //Move forward
			int64_t pawnMoveTwice = (pawnMove & rank3) << 8; //Move twice on first turn if first is clear
			int64_t pawnAttacks = board->whitePawnBitboard << 7 & board->blackPieces & fileH | board->whitePawnBitboard << 9 & board->blackPieces & ~fileA; //Attack
		}
	}
	else
	{

	}
}
