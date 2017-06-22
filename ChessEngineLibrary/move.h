#pragma once
#include <string>
#include <stdint.h>

#include "piece.h"
#include "board.h"

#include "transpositionTable.h"

enum MoveType{quietMove, capture, knightPromotion, bishopPromotion, rookPromotion, queenPromotion, pawnDoubleMove, kingSideCastling, queenSideCastling};

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

void updateCastlingRights(Board* newBoard, Move* move);


