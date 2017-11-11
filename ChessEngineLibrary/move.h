#pragma once
#include <string>
#include <stdint.h>

#include "piece.h"
#include "board.h"

#include "transpositionTable.h"

enum MoveType{quietMove, capture, knightPromotion, bishopPromotion, rookPromotion, queenPromotion, kingSideCastling, queenSideCastling};

struct Move
{
	Move();
	Move(int newFrom, int newTo, MoveType newMoveType, pieceType newPieceType, Board* board);
	int from;
	int to;
	pieceType piece;
	pieceType capturedPiece;
	MoveType moveType;

	int moveRating;
	
	//The flags of the board prior to applyMove. Used in undoMove
	bool canBlackCastleQueenSide;
	bool canBlackCastleKingSide;
	bool canWhiteCastleQueenSide;
	bool canWhiteCastleKingSide;
	int enPassantSquare;
	uint64_t hash;
	uint64_t pawnHash;
	int fiftyMoveTimer;

	void applyMove(Board* board);
	void undoMove(Board* board);

	//Updates all keys used for hash tables.
	void updateZorbistKeys(Board* board, colours opponentColour);

	bool operator==(const Move& b)
	{
		return (from == b.from) && (to == b.to) && (piece == b.piece) && (moveType == b.moveType);
	}
};

void updateCastlingRights(Board* newBoard, Move* move);



