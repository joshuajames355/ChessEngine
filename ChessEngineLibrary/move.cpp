#include "move.h"

Move::Move()
{
}

Move::Move(int newFrom, int newTo, MoveType newMoveType, pieceType newPieceType, Board* board)
{
	moveType = newMoveType;
	to = newTo;
	from = newFrom;
	piece = newPieceType;
	moveRating = 0;

	if (board->allPieces & (uint64_t)1 << to)
	{
		capturedPiece = board->getPieceTypeInSquare((uint64_t)1 << to);
	}
	else
	{
		capturedPiece = blank;
	}

	canBlackCastleQueenSide = board->canBlackCastleQueenSide;
	canBlackCastleKingSide = board->canBlackCastleKingSide;
	canWhiteCastleQueenSide = board->canWhiteCastleQueenSide;
	canWhiteCastleKingSide = board->canWhiteCastleKingSide;
	enPassantSquare = board->enPassantSquare;
	hash = board->zorbistKey;
}

void Move::applyMove(Board * board)
{
	updateCastlingRights(board, this);

	colours opponentColour = switchColour(board->nextColour);

	//Removes en passant file from hash.
	if (board->enPassantSquare != -1)
	{
		board->zorbistKey ^= ZorbistKeys::enPassantKeys[board->enPassantSquare % 8];
	}

	if (moveType != capture)
	{
		board->enPassantSquare = -1;
	}

	//Removes moved piece from hash.
	board->zorbistKey ^= ZorbistKeys::pieceKeys[from][piece + 6 * board->nextColour];

	//Removed captured piece from hash
	if (board->allPieces & (uint64_t)1 << to)
	{
		board -> zorbistKey ^= ZorbistKeys::pieceKeys[to][capturedPiece + 6 * opponentColour];
	}

	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);

		//Adds moved piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}
	break;
	case capture:
	{
		if (board->enPassantSquare == to && piece == pawn)
		{
			//Removes the captued piece under en passent
			if (board->nextColour == white)
			{
				board->removePiece((uint64_t)1 << (to - 8));
				board->zorbistKey ^= ZorbistKeys::pieceKeys[to - 8][pawn + 6 * opponentColour];
			}
			else
			{
				board->removePiece((uint64_t)1 << (to + 8));
				board->zorbistKey ^= ZorbistKeys::pieceKeys[to + 8][pawn + 6 * opponentColour];
			}
		}
		else
		{
			//Removes the captued piece
			board->removePiece((uint64_t)1 << to);
		}

		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);

		//Adds moved piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];

		board->enPassantSquare = -1;
	}
	break;
	case knightPromotion:
	{
		//Removes the captued piece
		board->removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		board->removePiece((uint64_t)1 << from);

		//Creates the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, knight);
		bitboard |= ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, knight, bitboard);

		//Adds promoted piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][knight + 6 * board->nextColour];
	}
	break;
	case bishopPromotion:
	{
		//Removes the captued piece
		board->removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		board->removePiece((uint64_t)1 << from);

		//Creates the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, bishop);
		bitboard |= ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, bishop, bitboard);

		//Adds promoted piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][bishop + 6 * board->nextColour];
	}
	break;
	case rookPromotion:
	{
		//Removes the captued piece
		board->removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		board->removePiece((uint64_t)1 << from);

		//Creates the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, rook);
		bitboard |= ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, rook, bitboard);

		//Adds promoted piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][rook + 6 * board->nextColour];
	}
	break;
	case queenPromotion:
	{
		//Removes the captued piece
		board->removePiece((uint64_t)1 << to);

		//Removes the moved Piece
		board->removePiece((uint64_t)1 << from);

		//Creates the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, queen);
		bitboard |= ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, queen, bitboard);

		//Adds promoted piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][queen + 6 * board->nextColour];
	}
	break;
	case pawnDoubleMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);

		//Sets En passant target square and hash.
		if (board->nextColour == white)
		{
			board->enPassantSquare = to - 8;
			board->zorbistKey ^= ZorbistKeys::enPassantKeys[board->enPassantSquare % 8];
		}
		else
		{
			board->enPassantSquare = to + 8;
			board->zorbistKey ^= ZorbistKeys::enPassantKeys[board->enPassantSquare % 8];
		}

		//Adds moved piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}
	break;
	case kingSideCastling:
	{
		//Moves the king
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			board->setBitboard(white, rook ,(board->getPieceBitboard(white, rook) & ~128) | 32); //Moves the rook

			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[7][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[5][rook + 6 * board->nextColour];
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~9223372036854775808) | 2305843009213693952); //Moves the rook

			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[63][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[61][rook + 6 * board->nextColour];
		}

		//Adds moved piece to hash
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}
	break;
	case queenSideCastling:
	{
		//Moves the king
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			board->setBitboard(white, rook, (board->getPieceBitboard(white, rook) & ~1) | 8); //Moves the rook

			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[0][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[3][rook + 6 * board->nextColour];
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~72057594037927936) | 576460752303423488); //Moves the rook

			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[56][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[59][rook + 6 * board->nextColour];
		}
	}
	break;
	}
	board->nextMove();
}

//Updates castling rights and the zorbist hash keys for castling rights.
void updateCastlingRights(Board * newBoard, Move * move)
{
	if (move->piece == king)
	{
		if (newBoard->nextColour == white)
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
		if (newBoard->nextColour == white)
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
		if (((uint64_t)1 << move->to & newBoard->getPieceBitboard(white,rook)) > 0)
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
		else if (((uint64_t)1 << move->to & newBoard->getPieceBitboard(black, rook)) > 0)
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

void Move::undoMove(Board * board)
{
	board->moveHistory.pop_back();

	colours opponentColour = board->nextColour;
	board->nextColour = switchColour(board->nextColour);
	board->kingDangerSquares = 0;
	board->canBlackCastleQueenSide = canBlackCastleQueenSide;
	board->canBlackCastleKingSide = canBlackCastleKingSide;
	board->canWhiteCastleQueenSide = canWhiteCastleQueenSide;
	board->canWhiteCastleKingSide = canWhiteCastleKingSide;
	board->zorbistKey = hash;
	board->enPassantSquare = enPassantSquare;

	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);
	}
	break;
	case capture:
	{
		if (board->enPassantSquare == to && piece == pawn)
		{
			//Adds the captured piece under en passent
			if (board->nextColour == white)
			{
				board->setBitboard(opponentColour, pawn, board->getPieceBitboard(opponentColour, pawn) | ((uint64_t)1 << (to - 8)));
			}
			else
			{
				board->setBitboard(opponentColour, pawn, board->getPieceBitboard(opponentColour, pawn) | ((uint64_t)1 << (to + 8)));
			}
		}
		else
		{
			//Adds the captured piece
			board->setBitboard(opponentColour, capturedPiece, board->getPieceBitboard(opponentColour, capturedPiece) | ((uint64_t)1 << to ));
		}

		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);
	}
	break;
	case knightPromotion:
	{
		//Adds the captued piece
		if(capturedPiece != blank)
			board->setBitboard(opponentColour, capturedPiece, board->getPieceBitboard(opponentColour, capturedPiece) | ((uint64_t)1 << to));

		//Adds the moved Piece
		board->setBitboard(board->nextColour, piece, board->getPieceBitboard(board->nextColour, piece) | ((uint64_t)1 << from));

		//Removes the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, knight);
		bitboard &= ~((uint64_t)1 << to);
		board->setBitboard(board->nextColour, knight, bitboard);
	}
	break;
	case bishopPromotion:
	{
		//Adds the captued piece
		if (capturedPiece != blank)
			board->setBitboard(opponentColour, capturedPiece, board->getPieceBitboard(opponentColour, capturedPiece) | ((uint64_t)1 << to));

		//Adds the moved Piece
		board->setBitboard(board->nextColour, piece, board->getPieceBitboard(board->nextColour, piece) | ((uint64_t)1 << from));

		//Removes the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, bishop);
		bitboard &= ~((uint64_t)1 << to);
		board->setBitboard(board->nextColour, bishop, bitboard);
	}
	break;
	case rookPromotion:
	{
		//Adds the captued piece
		if (capturedPiece != blank)
			board->setBitboard(opponentColour, capturedPiece, board->getPieceBitboard(opponentColour, capturedPiece) | ((uint64_t)1 << to));

		//Adds the moved Piece
		board->setBitboard(board->nextColour, piece, board->getPieceBitboard(board->nextColour, piece) | ((uint64_t)1 << from));

		//Removes the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, rook);
		bitboard &= ~((uint64_t)1 << to);
		board->setBitboard(board->nextColour, rook, bitboard);
	}
	break;
	case queenPromotion:
	{
		//Adds the captued piece
		if (capturedPiece != blank)
			board->setBitboard(opponentColour, capturedPiece, board->getPieceBitboard(opponentColour, capturedPiece) | ((uint64_t)1 << to));

		//Adds the moved Piece
		board->setBitboard(board->nextColour, piece, board->getPieceBitboard(board->nextColour, piece) | ((uint64_t)1 << from));

		//Removes the promoted piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, queen);
		bitboard &= ~((uint64_t)1 << to);
		board->setBitboard(board->nextColour, queen, bitboard);
	}
	break;
	case pawnDoubleMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);
	}
	break;
	case kingSideCastling:
	{
		//Moves the king
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			board->setBitboard(white, rook, (board->getPieceBitboard(white, rook) & ~32) | 128); //Moves the rook
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~2305843009213693952) | 9223372036854775808); //Moves the rook
		}
	}
	break;
	case queenSideCastling:
	{
		//Moves the king
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);
		if (board->nextColour == white)
		{
			board->setBitboard(white, rook, (board->getPieceBitboard(white, rook) & ~8) | 1); //Moves the rook
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~576460752303423488) | 72057594037927936); //Moves the rook

		}
	}
	break;
	}
	board->update();
}