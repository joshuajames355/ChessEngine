#pragma once
#include <string>

#include "piece.h"
#include "board.h"

enum MoveType{quietMove,capture,knightPromotion,bishopPromotion,rookPromotion,queenPromotion};

struct Move
{
	Move();
	Move(int newFrom , int newTo , MoveType newMoveType,pieceType newPieceType);
	int from;
	int to;
	pieceType piece;
	MoveType moveType;
	
	Board applyMove(Board* board);

	bool operator==(const Move& b)
	{
		return (from == b.from) && (to == b.to) && (piece = b.piece) && (moveType = b.moveType);
	}
};


