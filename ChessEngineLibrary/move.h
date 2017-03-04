#pragma once
#include <string>

#include "piece.h"
#include "board.h"

enum MoveType{quietMove,pawnCapture,pawnDoubleMove};

class move
{
public:
	move(int newFrom , int newTo , MoveType newMoveType , pieceType pieceType, Board* baseBoard);
	int from;
	int to;
	MoveType moveType;
	Board board;
};


