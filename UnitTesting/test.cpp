#pragma once

#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"
#include "move.h"

#include <iostream>
#include <stdint.h>


TEST(BoardTest, UpdateFunction)
{
	Board board;
	board.whitePawnBitboard = 2;
	board.whiteBishopBitboard = 4;
	board.update();
	ASSERT_EQ(board.allPieces, 6);

	board.blackBishopBitboard = 8;
	board.update();
	ASSERT_EQ(board.allPieces, 14);
	ASSERT_EQ(board.whitePieces, 6);
	ASSERT_EQ(board.blackPieces, 8);
}
TEST(BoardTest, Defaults)
{
	Board board;
	board.defaults();
	ASSERT_EQ(board.whitePawnBitboard, 65280);
	ASSERT_EQ(board.whiteRookBitboard, 129);
	ASSERT_EQ(board.whiteKnightBitboard, 66);
	ASSERT_EQ(board.whiteBishopBitboard, 36);
	ASSERT_EQ(board.whiteQueenBitboard, 8);
	ASSERT_EQ(board.whiteKingBitboard, 16);

	ASSERT_EQ(board.blackPawnBitboard, 71776119061217280);
	ASSERT_EQ(board.blackRookBitboard, 9295429630892703744);
	ASSERT_EQ(board.blackKnightBitboard, 4755801206503243776);
	ASSERT_EQ(board.blackBishopBitboard, 2594073385365405696);
	ASSERT_EQ(board.blackQueenBitboard, 576460752303423488);
	ASSERT_EQ(board.blackKingBitboard, 1152921504606846976);

}

TEST(BitboardTest, bitsum)
{
	ASSERT_EQ(bitSum(15), 4);
	ASSERT_EQ(bitSum(17), 2);
	ASSERT_EQ(bitSum(emptyBitboard), 0);
	ASSERT_EQ(bitSum(universalBitboard), 64);
	ASSERT_EQ(bitSum(255), 8);
}

TEST(BitboardTest, pop)
{
	uint64_t b = 15;
	ASSERT_EQ(pop(b), 1);
	ASSERT_EQ(pop(b), 2);
	ASSERT_EQ(pop(b), 4);
	ASSERT_EQ(pop(b), 8);
	ASSERT_EQ(pop(b), 0);

	b = 263;
	ASSERT_EQ(pop(b), 1);
	ASSERT_EQ(pop(b), 2);
	ASSERT_EQ(pop(b), 4);
	ASSERT_EQ(pop(b), 256);
	ASSERT_EQ(pop(b), 0);
}

TEST(ScoringTest, material)
{
	Board board;
	board.whitePawnBitboard = 1;
	ASSERT_EQ(calculateMaterialScore(&board, white), 100);
	ASSERT_EQ(calculateMaterialScore(&board, black), -100);

	board.whitePawnBitboard = 0;
	board.blackKnightBitboard = 5;
	ASSERT_EQ(calculateMaterialScore(&board, white), -640);
	ASSERT_EQ(calculateMaterialScore(&board, black), 640);

	board.blackKnightBitboard = 0;
	board.whiteQueenBitboard= 8;
	ASSERT_EQ(calculateMaterialScore(&board, white), 900);
	ASSERT_EQ(calculateMaterialScore(&board, black), -900);

	board.defaults();
	ASSERT_EQ(calculateMaterialScore(&board, black), 0);
	ASSERT_EQ(calculateMaterialScore(&board, white), 0);
}

TEST(ScoringTest, positionalScore)
{
	Board board;
	board.whiteRookBitboard = 1;
	board.update();
	ASSERT_EQ(calculatePositionalScore(&board, white), 0);
	ASSERT_EQ(calculatePositionalScore(&board, black), 0);
	
	board = Board();
	board.whitePawnBitboard = 512;
	board.update();
	ASSERT_EQ(calculatePositionalScore(&board, white) , 10);
	ASSERT_EQ(calculatePositionalScore(&board, black), -10);
	
}

TEST(MoveTest, ApplyMove)
{
	Move move = Move(1, 9, quietMove, pawn);
	Board board;
	board.whitePawnBitboard = 2;
	Board moveBoard = move.applyMove(&board, white);
	ASSERT_EQ(moveBoard.whitePawnBitboard, 1 << 9);
	ASSERT_EQ(moveBoard.allPieces, 1 << 9);

	board = Board();
	move = Move(1, 7, quietMove, queen);
	board.whiteQueenBitboard = 2;
	moveBoard = move.applyMove(&board, white);
	ASSERT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	ASSERT_EQ(moveBoard.allPieces, 1 << 7);

}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

