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
		newBoard.removePiece((uint64_t)1 << to); //Removes the captued piece

		uint64_t bitboard = newBoard.findBitboard(colour, piece); //Moves the piece
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(colour, piece, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	case knightPromotion:
	{
		Board newBoard = *board;
		newBoard.removePiece((uint64_t)1 << from); //Removes the moved Piece

		uint64_t bitboard = newBoard.findBitboard(colour, knight); //Creates the promoted piece
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(colour, knight, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	case bishopPromotion:
	{
		Board newBoard = *board;
		newBoard.removePiece((uint64_t)1 << from); //Removes the moved Piece

		uint64_t bitboard = newBoard.findBitboard(colour, bishop); //Creates the promoted piece
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(colour, bishop, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	case rookPromotion:
	{
		Board newBoard = *board;
		newBoard.removePiece((uint64_t)1 << from); //Removes the moved Piece

		uint64_t bitboard = newBoard.findBitboard(colour, rook); //Creates the promoted piece
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(colour, rook, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	case queenPromotion:
	{
		Board newBoard = *board;
		newBoard.removePiece((uint64_t)1 << from); //Removes the moved Piece

		uint64_t bitboard = newBoard.findBitboard(colour, queen); //Creates the promoted piece
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(colour, queen, bitboard);
		newBoard.update();
		return newBoard;
	}
	break;
	}
}
