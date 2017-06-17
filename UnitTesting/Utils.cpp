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
	EXPECT_EQ(notationFromMove(Move(48, 56, knightPromotion, pawn)), "a7a8n");
	EXPECT_EQ(notationFromMove(Move(8, 0, queenPromotion, pawn)), "a2a1q");

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

	board = Board();
	board.whitePawnBitboard = 1;
	board.blackPawnBitboard = 512;
	board.update();
	move = moveFromNotation("a1b2", &board);
	EXPECT_EQ(move.from, 0);
	EXPECT_EQ(move.to, 9);
	EXPECT_EQ(move.moveType, capture);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.whitePawnBitboard = 281474976710656;
	board.update();
	move = moveFromNotation("a7a8q", &board);
	EXPECT_EQ(move.from, 48);
	EXPECT_EQ(move.to, 56);
	EXPECT_EQ(move.moveType, queenPromotion);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.blackPawnBitboard = 256;
	board.update();
	move = moveFromNotation("a2a1n", &board);
	EXPECT_EQ(move.from, 8);
	EXPECT_EQ(move.to, 0);
	EXPECT_EQ(move.moveType, knightPromotion);
	EXPECT_EQ(move.piece, pawn);

	board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 128;
	board.update();
	move = moveFromNotation("e1g1", &board);
	EXPECT_EQ(move.from,4);
	EXPECT_EQ(move.to, 6);
	EXPECT_EQ(move.moveType, kingSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 1;
	board.update();
	move = moveFromNotation("e1c1", &board);
	EXPECT_EQ(move.from, 4);
	EXPECT_EQ(move.to, 2);
	EXPECT_EQ(move.moveType, queenSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 9223372036854775808;
	board.update();
	move = moveFromNotation("e8g8", &board);
	EXPECT_EQ(move.from, 60);
	EXPECT_EQ(move.to, 62);
	EXPECT_EQ(move.moveType, kingSideCastling);
	EXPECT_EQ(move.piece, king);

	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 72057594037927936;
	board.update();
	move = moveFromNotation("e8c8", &board);
	EXPECT_EQ(move.from, 60);
	EXPECT_EQ(move.to, 58);
	EXPECT_EQ(move.moveType, queenSideCastling);
	EXPECT_EQ(move.piece, king);
}

TEST(Utils, isInCheck)
{
	Board board = Board();
	board.blackKingBitboard = 1;
	board.whiteRookBitboard = 8;
	board.update();
	EXPECT_EQ(isInCheck(&board, black), true);

	board = Board();
	board.blackKingBitboard = 256;
	board.whiteRookBitboard = 8;
	board.update();
	EXPECT_EQ(isInCheck(&board, black), false);
}
