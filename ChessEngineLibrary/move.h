#pragma once
#include <string>

#include "piece.h"
#include "board.h"

enum MoveType{quietMove,capture};

struct Move
{
	Move(int newFrom , int newTo , MoveType newMoveType,pieceType newPieceType);
	int from;
	int to;
	pieceType piece;
	MoveType moveType;
	
	Board applyMove(Board* board , colours colour);
};


