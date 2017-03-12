#include "move.h"

Move::Move()
{
}

Move::Move(int newFrom, int newTo, MoveType newMoveType, pieceType newPieceType)
{
	moveType = newMoveType;
	to = newTo;
	from = newFrom;
	piece = newPieceType;
}

Board Move::applyMove(Board * board, colours colour)
{
	switch (moveType)
	{
	case quietMove:
	{
		Board newBoard = *board;  //Moves the piece
		uint64_t bitboard = newBoard.findBitboard(colour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(colour, piece, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	case capture:
	{
		Board newBoard = *board;
		newBoard.removePiece(1 << to); //Removes the captued piece

		uint64_t bitboard = newBoard.findBitboard(colour, piece); //Moves the piece
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(colour, piece, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	}
}
