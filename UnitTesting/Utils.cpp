#pragma once
#include "gtest/gtest.h"
#include "utils.h"
#include "move.h"

TEST(Utils, notationFromMove)
{
	Board board;
	EXPECT_EQ(notationFromMove(Move(0, 8, quietMove, pawn, &board)), "a1a2");
	EXPECT_EQ(notationFromMove(Move(3, 11, quietMove, pawn, &board)), "d1d2");
	EXPECT_EQ(notationFromMove(Move(32, 40, quietMove, pawn, &board)), "a5a6");
	EXPECT_EQ(notationFromMove(Move(54, 62, quietMove, pawn, &board)), "g7g8");
	EXPECT_EQ(notationFromMove(Move(48, 56, knightPromotion, pawn, &board)), "a7a8n");
	EXPECT_EQ(notationFromMove(Move(8, 0, queenPromotion, pawn, &board)), "a2a1q");

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
	EXPECT_EQ(move.moveType, pawnDoubleMove);
	EXPECT_EQ(move.piece, pawn);

	move = moveFromNotation("a7a6", &board);
	EXPECT_EQ(move.from, 48);
	EXPECT_EQ(move.to, 40);
	EXPECT_EQ(move.moveType, quietMove);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.setBitboard(white, bishop, 2);
	board.setBitboard(black, pawn, 256);
	board.update();
	move = moveFromNotation("b1a2", &board);
	EXPECT_EQ(move.from, 1);
	EXPECT_EQ(move.to, 8);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, bishop);

	board = Board();
	board.setBitboard(black, king, 34359738368);
	board.setBitboard(white, pawn, 134217728);
	board.update();
	move = moveFromNotation("d5d4", &board);
	EXPECT_EQ(move.from, 35);
	EXPECT_EQ(move.to, 27);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.setBitboard(white, pawn, 1);
	board.setBitboard(black, pawn, 512);
	board.update();
	move = moveFromNotation("a1b2", &board);
	EXPECT_EQ(move.from, 0);
	EXPECT_EQ(move.to, 9);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.setBitboard(white, pawn, 281474976710656);
	board.update();
	move = moveFromNotation("a7a8q", &board);
	EXPECT_EQ(move.from, 48);
	EXPECT_EQ(move.to, 56);
	EXPECT_EQ(move.moveType, queenPromotion);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.setBitboard(black, pawn, 256);
	board.update();
	move = moveFromNotation("a2a1n", &board);
	EXPECT_EQ(move.from, 8);
	EXPECT_EQ(move.to, 0);
	EXPECT_EQ(move.moveType, knightPromotion);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 128);
	board.update();
	move = moveFromNotation("e1g1", &board);
	EXPECT_EQ(move.from,4);
	EXPECT_EQ(move.to, 6);
	EXPECT_EQ(move.moveType, kingSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 1);
	board.update();
	move = moveFromNotation("e1c1", &board);
	EXPECT_EQ(move.from, 4);
	EXPECT_EQ(move.to, 2);
	EXPECT_EQ(move.moveType, queenSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 9223372036854775808);
	board.update();
	move = moveFromNotation("e8g8", &board);
	EXPECT_EQ(move.from, 60);
	EXPECT_EQ(move.to, 62);
	EXPECT_EQ(move.moveType, kingSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 72057594037927936);
	board.update();
	move = moveFromNotation("e8c8", &board);
	EXPECT_EQ(move.from, 60);
	EXPECT_EQ(move.to, 58);
	EXPECT_EQ(move.moveType, queenSideCastling);
	EXPECT_EQ(move.piece, king);
}
