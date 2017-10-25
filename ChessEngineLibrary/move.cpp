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
	pawnHash = board->pawnScoreZorbistKey;
}

void Move::applyMove(Board * board)
{
	colours opponentColour = switchColour(board->nextColour);

	updateCastlingRights(board, this);
	updateZorbistKeys(board, opponentColour);

	board->removePositionalScore(board->nextColour, piece, from);

	if (moveType != capture) board->enPassantSquare = -1;
	
	//Updates materialScore for removed pieces
	if (capturedPiece != blank)
	{
		board->removeMaterialScore(opponentColour, capturedPiece);
		board->removePositionalScore(opponentColour, capturedPiece, to);
	}
	
	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << from)) | ((uint64_t)1 << to);
		board->setBitboard(board->nextColour, piece, bitboard);

		board->addPositionalScore(board->nextColour, piece, to);
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
				board->removePositionalScore(opponentColour, pawn, to - 8);
			}
			else
			{
				board->removePiece((uint64_t)1 << (to + 8));
				board->removePositionalScore(opponentColour, pawn, to + 8);
			}
			board->removeMaterialScore(opponentColour, pawn);
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
		board->enPassantSquare = -1;

		board->addPositionalScore(board->nextColour, piece, to);
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

		board->removeMaterialScore(board->nextColour, pawn);
		board->addMaterialScore(board->nextColour, knight);
		board->addPositionalScore(board->nextColour, knight, to);
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

		board->removeMaterialScore(board->nextColour, pawn);
		board->addMaterialScore(board->nextColour, bishop);
		board->addPositionalScore(board->nextColour, bishop, to);
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

		board->removeMaterialScore(board->nextColour, pawn);
		board->addMaterialScore(board->nextColour, rook);
		board->addPositionalScore(board->nextColour, rook, to);
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

		board->removeMaterialScore(board->nextColour, pawn);
		board->addMaterialScore(board->nextColour, queen);
		board->addPositionalScore(board->nextColour, queen, to);
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
		}
		else
		{
			board->enPassantSquare = to + 8;
		}

		board->addPositionalScore(board->nextColour, piece, to);
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
			//Moves the rook
			board->setBitboard(white, rook ,(board->getPieceBitboard(white, rook) & ~128) | 32); 

			board->removePositionalScore(white, rook, 7);
			board->addPositionalScore(white, rook, 5);
		}
		else
		{
			//Moves the rook
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~9223372036854775808) | 2305843009213693952); 

			board->removePositionalScore(white, rook, 63);
			board->addPositionalScore(white, rook, 61);
		}

		board->addPositionalScore(board->nextColour, piece, to);
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
			//Moves the rook
			board->setBitboard(white, rook, (board->getPieceBitboard(white, rook) & ~1) | 8); 

			board->removePositionalScore(white, rook, 0);
			board->addPositionalScore(white, rook, 3);
		}
		else
		{
			//Moves the rook
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~72057594037927936) | 576460752303423488);

			board->removePositionalScore(white, rook, 56);
			board->addPositionalScore(white, rook, 59);
		}

		board->addPositionalScore(board->nextColour, piece, to);
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
	board->pawnScoreZorbistKey = pawnHash;
	board->enPassantSquare = enPassantSquare;

	board->addPositionalScore(board->nextColour, piece, from);

	//Updates materialScore for removed pieces
	if (capturedPiece != blank)
	{
		board->addMaterialScore(opponentColour, capturedPiece);
		board->addPositionalScore(opponentColour, capturedPiece, to);
	}

	switch (moveType)
	{
	case quietMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);

		board->removePositionalScore(board->nextColour, piece, to);
	}
	break;
	case capture:
	{
		if (board->enPassantSquare == to)
		{
			//Adds the captured piece under en passent
			if (board->nextColour == white)
			{
				board->setBitboard(opponentColour, pawn, board->getPieceBitboard(opponentColour, pawn) | ((uint64_t)1 << (to - 8)));
				board->addPositionalScore(opponentColour, pawn, to - 8);
			}
			else
			{
				board->setBitboard(opponentColour, pawn, board->getPieceBitboard(opponentColour, pawn) | ((uint64_t)1 << (to + 8)));
				board->addPositionalScore(opponentColour, pawn, to + 8);
			}
			board->addMaterialScore(opponentColour, pawn);
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

		board->removePositionalScore(board->nextColour, piece, to);
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

		board->addMaterialScore(board->nextColour, pawn);
		board->removeMaterialScore(board->nextColour, knight);
		board->removePositionalScore(board->nextColour, knight, to);
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

		board->addMaterialScore(board->nextColour, pawn);
		board->removeMaterialScore(board->nextColour, bishop);
		board->removePositionalScore(board->nextColour, bishop, to);
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

		board->addMaterialScore(board->nextColour, pawn);
		board->removeMaterialScore(board->nextColour, rook);
		board->removePositionalScore(board->nextColour, rook, to);
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

		board->addMaterialScore(board->nextColour, pawn);
		board->removeMaterialScore(board->nextColour, queen);
		board->removePositionalScore(board->nextColour, queen, to);
	}
	break;
	case pawnDoubleMove:
	{
		//Moves the piece
		uint64_t bitboard = board->getPieceBitboard(board->nextColour, piece);
		bitboard = (bitboard & ~((uint64_t)1 << to)) | ((uint64_t)1 << from);
		board->setBitboard(board->nextColour, piece, bitboard);

		board->removePositionalScore(board->nextColour, piece, to);
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

			board->addPositionalScore(white, rook, 7);
			board->removePositionalScore(white, rook, 5);
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~2305843009213693952) | 9223372036854775808); //Moves the rook
		
			board->addPositionalScore(white, rook, 63);
			board->removePositionalScore(white, rook, 61);
		}

		board->removePositionalScore(board->nextColour, piece, to);
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
			board->addPositionalScore(white, rook, 0);
			board->removePositionalScore(white, rook, 3);
		}
		else
		{
			board->setBitboard(black, rook, (board->getPieceBitboard(black, rook) & ~576460752303423488) | 72057594037927936); //Moves the rook
			board->addPositionalScore(white, rook, 56);
			board->removePositionalScore(white, rook, 59);
		}
		board->removePositionalScore(board->nextColour, piece, to);
	}
	break;
	}
	board->update();
}

void Move::updateZorbistKeys(Board * board, colours opponentColour)
{
	//Removes en passant file from hash.
	if (board->enPassantSquare != -1)
	{
		board->zorbistKey ^= ZorbistKeys::enPassantKeys[board->enPassantSquare % 8];
	}

	//moves piece in hash
	board->zorbistKey ^= ZorbistKeys::pieceKeys[from][piece + 6 * board->nextColour];
	board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];

	//updates pawn structure hash
	if (piece == pawn)
	{
		board->pawnScoreZorbistKey ^= ZorbistKeys::pieceKeys[from][piece + 6 * board->nextColour];
		board->pawnScoreZorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
	}

	//Removed captured piece from hash when capture is not enpassant
	if (capturedPiece != blank && to != enPassantSquare)
	{
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][capturedPiece + 6 * opponentColour];

		//Updates pawn structure hash
		if (capturedPiece == pawn)
		{
			board->pawnScoreZorbistKey ^= ZorbistKeys::pieceKeys[to][capturedPiece + 6 * opponentColour];
		}
	}

	switch (moveType)
	{
	case capture:
	{
		if (board->enPassantSquare == to && piece == pawn)
		{
			//Removes the captured piece's hash under en passent
			if (board->nextColour == white)
			{
				board->zorbistKey ^= ZorbistKeys::pieceKeys[to - 8][pawn + 6 * opponentColour];
				board->pawnScoreZorbistKey ^= ZorbistKeys::pieceKeys[to - 8][pawn + 6 * opponentColour];
			}
			else
			{
				board->zorbistKey ^= ZorbistKeys::pieceKeys[to + 8][pawn + 6 * opponentColour];
				board->pawnScoreZorbistKey ^= ZorbistKeys::pieceKeys[to + 8][pawn + 6 * opponentColour];
			}
		}
	}
	break;
	case knightPromotion:
	{
		//Switches hash to promoted pieceType
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][knight + 6 * board->nextColour];
	}
	break;
	case bishopPromotion:
	{
		//Switches hash to promoted pieceType
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][bishop + 6 * board->nextColour];
	}
	break;
	case rookPromotion:
	{
		//Switches hash to promoted pieceType
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][rook + 6 * board->nextColour];
	}
	break;
	case queenPromotion:
	{
		//Switches hash to promoted pieceType
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][piece + 6 * board->nextColour];
		board->zorbistKey ^= ZorbistKeys::pieceKeys[to][queen + 6 * board->nextColour];
	}
	break;
	case pawnDoubleMove:
	{
		//Sets En passant target hash.
		if (board->nextColour == white)
		{
			board->zorbistKey ^= ZorbistKeys::enPassantKeys[to % 8];
		}
		else
		{
			board->zorbistKey ^= ZorbistKeys::enPassantKeys[to % 8];
		}
	}
	break;
	case kingSideCastling:
	{
		if (board->nextColour == white)
		{
			 //Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[7][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[5][rook + 6 * board->nextColour];
		}
		else
		{
			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[63][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[61][rook + 6 * board->nextColour];
		}
	}
	break;
	case queenSideCastling:
	{
		if (board->nextColour == white)
		{
			 //Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[0][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[3][rook + 6 * board->nextColour];
		}
		else
		{
			//Updates hash for rook
			board->zorbistKey ^= ZorbistKeys::pieceKeys[56][rook + 6 * board->nextColour];
			board->zorbistKey ^= ZorbistKeys::pieceKeys[59][rook + 6 * board->nextColour];
		}
	}
	break;
	}
}
