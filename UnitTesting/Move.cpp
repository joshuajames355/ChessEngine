#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "move.h"

TEST(Move, ApplyMove)
{
	Move move = Move(1, 9, quietMove, pawn);
	Board board;
	board.whitePawnBitboard = 2;
	Board moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 9);
	EXPECT_EQ(moveBoard.allPieces, 1 << 9);

	board = Board();
	move = Move(1, 7, quietMove, queen);
	board.whiteQueenBitboard = 2;
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);


	board = Board();
	move = Move(1, 7, capture, queen);
	board.whiteQueenBitboard = 2;
	board.blackPawnBitboard = 128;
	board.update();
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);

	board = Board();
	move = Move(1, 8, capture, pawn);
	board.whitePawnBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 8);
	EXPECT_EQ(moveBoard.allPieces, 1 << 8);
}