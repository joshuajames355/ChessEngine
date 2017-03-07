#include "moveGeneration.h"

std::vector<Move> searchForMoves(Board * board, colours aiColour)
{
	std::vector<Move> moveList;

	generatePawnMoves(board, aiColour, moveList);

	return moveList;
}

void generatePawnMoves(Board* board, colours aiColour, std::vector<Move>& Movelist)
{
	if (aiColour == white)
	{
		uint64_t pawnBitboard = board->whitePawnBitboard;
		uint64_t pawnPos, pawnMoves, pawnAttacks;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos << 8 & ~board->allPieces; //Move forward
			pawnMoves |= ((pawnMoves & rank3) << 8) & ~board->allPieces; //Move twice on first turn if first is clear
			pawnAttacks = board->whitePawnBitboard << 7 & board->blackPieces & fileH | board->whitePawnBitboard << 9 & board->blackPieces & ~fileA; //Attack

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnMoves)
			{
				uint64_t pawnMove = pop(pawnMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnMove), quietMove, pawn));
			}
			while (pawnAttacks)
			{
				uint64_t pawnAttack = pop(pawnAttacks);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnAttack), capture, pawn));
			}
		}
	}
	else //Colour is black
	{
		uint64_t pawnBitboard = board->blackPawnBitboard;
		uint64_t pawnPos, pawnMoves, pawnAttacks;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos >> 8 & ~board->allPieces; //Move forward
			pawnMoves |= ((pawnMoves & rank3) << 8) & ~board->allPieces;  //Move twice on first turn if first is clear
			pawnAttacks = board->blackPawnBitboard >> 9 & board->whitePieces & fileH | board->blackPawnBitboard >> 7 & board->whitePieces & ~fileA; //Attack

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnMoves)
			{
				uint64_t pawnMove = pop(pawnMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnMove), quietMove, pawn));
			}
			while (pawnAttacks)
			{
				uint64_t pawnAttack = pop(pawnAttacks);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnAttack), capture, pawn));
			}
		}
	}
}

void generateKingMoves(Board * board, colours aiColour, std::vector<Move>& Movelist)
{
	uint64_t kingBitboard , friendlyPieces;
	if (aiColour == white)
	{
		kingBitboard = board->whiteKingBitboard;
		friendlyPieces = board->whitePieces;
	}
	else
	{
		kingBitboard = board->blackKingBitboard;
		friendlyPieces = board->blackPieces;
	}
	uint64_t moves = kingBitboard << 8 & ~friendlyPieces;
	moves |= kingBitboard << 9 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard << 1 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard >> 7 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard >> 8 & ~friendlyPieces;
	moves |= kingBitboard >> 9 & ~friendlyPieces & ~fileH;
	moves |= kingBitboard >> 1 & ~friendlyPieces & ~fileH;
	moves |= kingBitboard << 7 & ~friendlyPieces & ~fileH;

	int kingPosIndex = bitScanForward(kingBitboard);
	while (moves)
	{
		uint64_t kingPos = pop(moves);
		addMoves(kingPosIndex, bitScanForward(kingPos), aiColour, king, Movelist, board);
	}

}

void addMoves(int start, int end, colours aiColour, pieceType piece, std::vector<Move>& Movelist, Board* board)
{
	uint64_t enemyPieces;
	if (aiColour == white)
	{
		enemyPieces = board->blackPieces;
	}
	else
	{
		enemyPieces = board->whitePieces;
	}
	if ((((uint64_t)1 << end) & enemyPieces )!= 0) //If the move is a capture
	{
		Movelist.push_back(Move(start, end, capture, piece));
	}
	else
	{
		Movelist.push_back(Move(start, end, quietMove, piece));
	}
}
