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

Board Move::applyMove(Board * board)
{
	Board newBoard = *board;
	newBoard.enPassantSquare = -1;
	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);
	}
	break;
	case capture:
	{
		if (board->enPassantSquare == to)
		{
			//Removes the captued piece under en passent
			if (board->nextColour == white)
			{
				newBoard.removePiece((uint64_t)1 << (to - 8));
			}
			else
			{
				newBoard.removePiece((uint64_t)1 << (to + 8));
			}
		}
		else
		{
			//Removes the captued piece
			newBoard.removePiece((uint64_t)1 << to);
		}

		//Moves the piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);
	}
	break;
	case knightPromotion:
	{
		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, knight);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, knight, bitboard);
	}
	break;
	case bishopPromotion:
	{
		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, bishop);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, bishop, bitboard);
	}
	break;
	case rookPromotion:
	{
		//Removes the moved Piece
		Board newBoard = *board;
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, rook);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, rook, bitboard);
	}
	break;
	case queenPromotion:
	{
		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, queen);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, queen, bitboard);
	}
	break;
	case pawnDoubleMove:
	{
		//Moves the piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);

		//Sets En passant target square.
		if (board->nextColour == white)
		{
			newBoard.enPassantSquare = to - 8;
		}
		else
		{
			newBoard.enPassantSquare = to + 8;
		}
	}
	break;
	}
	newBoard.nextColour = switchColour(newBoard.nextColour);
	newBoard.update();
	return newBoard;
}
