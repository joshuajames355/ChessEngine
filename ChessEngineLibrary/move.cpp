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
	pieceType capturedPiece;
	newBoard.enPassantSquare = -1;
	newBoard.nextColour = switchColour(newBoard.nextColour);

	updateCastlingRights(&newBoard, this);

	//Removes moved piece from hash.
	newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[from][piece + 6 * board->nextColour];

	//Removes en passant file from hash.
	if (board->enPassantSquare != -1)
	{
		newBoard.zorbistKey ^= ZorbistKeys::enPassantKeys[board->enPassantSquare % 8];
	}

	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);

		//Adds moved piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
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

		//Adds moved piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];

		//Removed captured piece from hash
		if (board->allPieces & (uint64_t)1 << to)
		{
			capturedPiece = newBoard.getPieceTypeInSquare((uint64_t)1 << to);
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * newBoard.nextColour];
		}
	}
	break;
	case knightPromotion:
	{
		//Removes the captued piece
		newBoard.removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, knight);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, knight, bitboard);

		//Adds promoted piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][knight + 6 * board->nextColour];

		//Removed captured piece from hash
		if (board->allPieces & (uint64_t)1 << to)
		{
			capturedPiece = newBoard.getPieceTypeInSquare((uint64_t)1 << to);
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * newBoard.nextColour];
		}
	}
	break;
	case bishopPromotion:
	{
		//Removes the captued piece
		newBoard.removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, bishop);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, bishop, bitboard);

		//Adds promoted piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][bishop + 6 * board->nextColour];

		//Removed captured piece from hash
		if (board->allPieces & (uint64_t)1 << to)
		{
			capturedPiece = newBoard.getPieceTypeInSquare((uint64_t)1 << to);
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * newBoard.nextColour];
		}
	}
	break;
	case rookPromotion:
	{
		//Removes the captued piece
		newBoard.removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, rook);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, rook, bitboard);

		//Adds promoted piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][rook + 6 * board->nextColour];

		//Removed captured piece from hash
		if (board->allPieces & (uint64_t)1 << to)
		{
			capturedPiece = newBoard.getPieceTypeInSquare((uint64_t)1 << to);
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * newBoard.nextColour];
		}
	}
	break;
	case queenPromotion:
	{
		//Removes the captued piece
		newBoard.removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		newBoard.removePiece((uint64_t)1 << from); 

		//Creates the promoted piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, queen);
		bitboard |= ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, queen, bitboard);

		//Adds promoted piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][queen + 6 * board->nextColour];

		//Removed captured piece from hash
		if (board->allPieces & (uint64_t)1 << to)
		{
			capturedPiece = newBoard.getPieceTypeInSquare((uint64_t)1 << to);
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * newBoard.nextColour];
		}
	}
	break;
	case pawnDoubleMove:
	{
		//Moves the piece
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);

		//Sets En passant target square and hash.
		if (board->nextColour == white)
		{
			newBoard.enPassantSquare = to - 8;
			newBoard.zorbistKey ^= ZorbistKeys::enPassantKeys[newBoard.enPassantSquare % 8];
		}
		else
		{
			newBoard.enPassantSquare = to + 8;
			newBoard.zorbistKey ^= ZorbistKeys::enPassantKeys[newBoard.enPassantSquare % 8];
		}

		//Adds moved piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}
	break;
	case kingSideCastling:
	{
		//Moves the king
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			newBoard.whiteRookBitboard = (newBoard.whiteRookBitboard & ~128) | 32; //Moves the rook

			//Updates hash for rook
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[7][rook + 6 * board->nextColour];
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[5][rook + 6 * board->nextColour];
		}
		else
		{
			newBoard.blackRookBitboard = (newBoard.blackRookBitboard & ~9223372036854775808) | 2305843009213693952; //Moves the rook

			//Updates hash for rook
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[63][rook + 6 * board->nextColour];
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[61][rook + 6 * board->nextColour];
		}

		//Adds moved piece to hash
		newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}
	break;
	case queenSideCastling:
	{
		//Moves the king
		uint64_t bitboard = newBoard.findBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		newBoard.setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			newBoard.whiteRookBitboard = (newBoard.whiteRookBitboard & ~1) | 8; //Moves the rook

			//Updates hash for rook
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[0][rook + 6 * board->nextColour];
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[3][rook + 6 * board->nextColour];
		}
		else
		{
			newBoard.blackRookBitboard = (newBoard.blackRookBitboard & ~72057594037927936) | 576460752303423488; //Moves the rook

			//Updates hash for rook
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[56][rook + 6 * board->nextColour];
			newBoard.zorbistKey ^= ZorbistKeys::pieceKeys[59][rook + 6 * board->nextColour];
		}
	}
	break;
	}

	newBoard.update();
	return newBoard;
}

//Updates castling rights and the zorbist hash keys for castling rights.
void updateCastlingRights(Board * newBoard, Move * move)
{
	if (move->piece == king)
	{
		if (newBoard->nextColour == black)
		{
			if(newBoard->canWhiteCastleKingSide)
			{
				newBoard->canWhiteCastleKingSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::whiteKingSideCastlingKey;
			}
			if (newBoard->canWhiteCastleQueenSide)
			{
				newBoard->canWhiteCastleQueenSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::whiteQueenSideCastlingKey;
			}
		}
		else
		{
			if (newBoard->canBlackCastleKingSide)
			{
				newBoard->canBlackCastleKingSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::blackKingSideCastlingKey;
			}
			if (newBoard->canBlackCastleQueenSide)
			{
				newBoard->canBlackCastleQueenSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::blackQueenSideCastlingKey;
			}
		}
	}
	else if (move->piece == rook)
	{
		if (newBoard->nextColour == black)
		{
			if (newBoard->canWhiteCastleQueenSide && move->from == 0)
			{
				newBoard->canWhiteCastleQueenSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::whiteQueenSideCastlingKey;
			}
			else if (newBoard->canWhiteCastleKingSide && move->from == 7)
			{
				newBoard->canWhiteCastleKingSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::whiteKingSideCastlingKey;
			}
		}
		else
		{
			if (newBoard->canBlackCastleQueenSide && move->from == 56)
			{
				newBoard->canBlackCastleQueenSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::blackQueenSideCastlingKey;
			}
			else if (newBoard->canBlackCastleKingSide && move->from == 63)
			{
				newBoard->canBlackCastleKingSide = false;
				newBoard->zorbistKey ^= ZorbistKeys::blackKingSideCastlingKey;
			}
		}
	}
	else if(move->moveType != quietMove)
	{
		//Capturing a rook
		if (((uint64_t)1 << move->to & newBoard->whiteRookBitboard) > 0)
		{
			if (move->to == 0)
			{
				if (newBoard->canWhiteCastleQueenSide)
				{
					newBoard->canWhiteCastleQueenSide = false;
					newBoard->zorbistKey ^= ZorbistKeys::whiteQueenSideCastlingKey;
				}
			}
			else if (move->to == 7)
			{
				if (newBoard->canWhiteCastleKingSide)
				{
					newBoard->canWhiteCastleKingSide = false;
					newBoard->zorbistKey ^= ZorbistKeys::whiteKingSideCastlingKey;
				}
			}
		}
		else if (((uint64_t)1 << move->to & newBoard->blackRookBitboard) > 0)
		{
			if (move->to == 56)
			{
				if (newBoard->canBlackCastleQueenSide)
				{
					newBoard->canBlackCastleQueenSide = false;
					newBoard->zorbistKey ^= ZorbistKeys::blackQueenSideCastlingKey;
				}
			}
			else if (move->to == 63)
			{
				if (newBoard->canBlackCastleKingSide)
				{
					newBoard->canBlackCastleKingSide = false;
					newBoard->zorbistKey ^= ZorbistKeys::blackKingSideCastlingKey;
				}
			}
		}
	}
}
