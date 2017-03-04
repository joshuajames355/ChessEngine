#include "move.h"

move::move(int newFrom, int newTo, MoveType newMoveType, pieceType pieceType, Board* baseBoard)
{
	moveType = newMoveType;
	board = *baseBoard;
	switch (pieceType)
	{
	case pawn:
		board.whitePawnBitboard -= 1 << newFrom;
		board.whitePawnBitboard += 1 << newTo;
	}
}
