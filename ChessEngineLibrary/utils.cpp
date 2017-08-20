#include "utils.h"

std::string notationFromMove(const Move & move)
{
	std::string notation = notationFromPiecePos(move.from) + notationFromPiecePos(move.to);

	if (move.moveType == knightPromotion)
	{
		notation += "n";
	}
	else if (move.moveType == rookPromotion)
	{
		notation += "r";
	}
	else if (move.moveType == bishopPromotion)
	{
		notation += "b";
	}
	else if (move.moveType == queenPromotion)
	{
		notation += "q";
	}
	
	return notation;
}

Move moveFromNotation(std::string moveNotation, Board * board)
{
	int column = moveNotation[0] - 'a';
	int row = moveNotation[1] - '1';
	int from = row * 8 + column;

	column = moveNotation[2] - 'a';
	row = moveNotation[3] - '1';
	int to = row * 8 + column;
	uint64_t fromBitboard = (uint64_t)1 << from;
	colours aiColour;
	pieceType piece = blank;
	if ((fromBitboard & board->whitePieces) != 0) //Is a white piece
	{
		aiColour = white;
		if ((fromBitboard & board->whiteBishopBitboard) != 0)
		{
			piece = bishop;
		}
		else if ((fromBitboard & board->whiteQueenBitboard) != 0)
		{
			piece = queen;
		}
		else if ((fromBitboard & board->whiteKingBitboard) != 0)
		{
			piece = king;
		}
		else if ((fromBitboard & board->whiteRookBitboard) != 0)
		{
			piece = rook;
		}
		else if ((fromBitboard & board->whiteKnightBitboard) != 0)
		{
			piece = knight;
		}
		else if ((fromBitboard & board->whitePawnBitboard) != 0)
		{
			piece = pawn;
		}
	}
	else if ((fromBitboard & board->blackPieces) != 0)
	{
		aiColour = black;
		if ((fromBitboard & board->blackBishopBitboard) != 0)
		{
			piece = bishop;
		}
		else if ((fromBitboard & board->blackQueenBitboard) != 0)
		{
			piece = queen;
		}
		else if ((fromBitboard & board->blackKingBitboard) != 0)
		{
			piece = king;
		}
		else if ((fromBitboard & board->blackRookBitboard) != 0)
		{
			piece = rook;
		}
		else if ((fromBitboard & board->blackKnightBitboard) != 0)
		{
			piece = knight;
		}
		else if ((fromBitboard & board->blackPawnBitboard) != 0)
		{
			piece = pawn;
		}
	}
	else
	{
		throw std::runtime_error("moveFromNotation failed. Piece not on board.");
	}

	if (std::abs(from - to) == 16 && piece == pawn)//Pawn double move
	{
		return Move(from, to, pawnDoubleMove, piece, board);
	}
	else if ((((uint64_t)1 << to) & board->allPieces) != 0 && moveNotation.length() == 4)//Capture
	{
		return Move(from, to, capture, piece, board);
	}
	else if (to == board->enPassantSquare && piece == pawn)//En passant capture
	{
		return Move(from, to, capture, piece, board);
	}
	else if (std::abs(from - to) == 2 && piece == king) //Castling
	{
		if (aiColour == white)
		{
			if (from < to) //KingSide castling
			{
				return Move(from, to, kingSideCastling, piece, board);
			}
			else //QueenSide castling
			{
				return Move(from, to, queenSideCastling, piece, board);
			}
		}
		else
		{
			if (from < to) //KingSide castling
			{
				return Move(from, to, kingSideCastling, piece, board);
			}
			else //QueenSide castling
			{
				return Move(from, to, queenSideCastling, piece, board);
			}
		}
	}
	else if (moveNotation.length() == 5) //Promotion moves. Format is a7a8q , where the last piece is the piece to be promoted to.
	{
		if (moveNotation.back() == 'q' || moveNotation.back() == 'Q')
		{
			return Move(from, to, queenPromotion, piece, board);
		}
		else if (moveNotation.back() == 'r' || moveNotation.back() == 'R')
		{
			return Move(from, to, rookPromotion, piece, board);
		}
		else if (moveNotation.back() == 'n' || moveNotation.back() == 'N')
		{
			return Move(from, to, knightPromotion, piece, board);
		}
		else if (moveNotation.back() == 'b' || moveNotation.back() == 'B')
		{
			return Move(from, to, bishopPromotion, piece, board);
		}

	}
	else//Quiet Move
	{
		return Move(from, to, quietMove, piece, board);
	}
}

std::string notationFromPiecePos(int piecePos)
{
	std::string notation;

	int column = (piecePos % 8);
	int row = 1 + piecePos / 8;

	char rowChar = row + '0';
	char columnChar = column + 'a';
	notation += columnChar;
	notation += rowChar;
	
	return notation;
}
