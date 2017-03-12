#include "utils.h"

std::string notationFromMove(Move & move)
{
	std::string ans;

	int column = (move.from % 8);
	int temp = move.from;
	int row = 1;
	for (; temp > 7; temp -= 8)
	{
		row++;
	}
	char rowChar = row + '0';
	char columnChar = column + 'a' ;
	ans += columnChar;
	ans += rowChar;

	column = (move.to % 8);
	temp = move.to;
	row = 1;
	for (; temp > 7; temp -= 8)
	{
		row++;
	}
	rowChar = row + '0';
	columnChar = column + 'a';
	ans += columnChar;
	ans += rowChar;
	
	return ans;
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
	pieceType piece;
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
	else
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
	if ((((uint64_t)1 << to) & board->allPieces) != 0)//Capture
	{
		return Move(from, to, capture, piece);
	}
	else//Quiet Move
	{
		return Move(from, to, quietMove, piece);
	}
}