#pragma once
#include "gtest/gtest.h"
#include "utils.h"
#include "move.h"

TEST(Utils, notationFromMove)
{
	EXPECT_EQ(notationFromMove(Move(0, 8, quietMove, pawn)), "a1a2");
	EXPECT_EQ(notationFromMove(Move(3, 11, quietMove, pawn)), "d1d2");
	EXPECT_EQ(notationFromMove(Move(32, 40, quietMove, pawn)), "a5a6");
	EXPECT_EQ(notationFromMove(Move(54, 62, quietMove, pawn)), "g7g8");
}

TEST(Utils, moveFromNotation)
{
	Board board = Board();
	board.defaults();
	Move move = moveFromNotation("a2a3", &board);
	EXPECT_EQ(move.from, 8);
	EXPECT_EQ(move.to, 16);
	EXPECT_EQ(move.moveType, quietMove);
	EXPECT_EQ(move.piece, pawn);

	move = moveFromNotation("h2h4", &board);
	EXPECT_EQ(move.from, 15);
	EXPECT_EQ(move.to, 31);
	EXPECT_EQ(move.moveType, quietMove);
	EXPECT_EQ(move.piece, pawn);

	move = moveFromNotation("a7a6", &board);
	EXPECT_EQ(move.from, 48);
	EXPECT_EQ(move.to, 40);
	EXPECT_EQ(move.moveType, quietMove);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.whiteBishopBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	move = moveFromNotation("b1a2", &board);
	EXPECT_EQ(move.from, 1);
	EXPECT_EQ(move.to, 8);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, bishop);

	board = Board();
	board.blackKingBitboard = 34359738368;
	board.whitePawnBitboard = 134217728;
	board.update();
	move = moveFromNotation("d5d4", &board);
	EXPECT_EQ(move.from, 35);
	EXPECT_EQ(move.to, 27);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, king);
}
