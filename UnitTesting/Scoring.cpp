#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"

TEST(Scoring, material)
{
	Board board;
	board.whitePawnBitboard = 1;
	EXPECT_EQ(calculateMaterialScore(&board, white), 100);
	EXPECT_EQ(calculateMaterialScore(&board, black), -100);

	board.whitePawnBitboard = 0;
	board.blackKnightBitboard = 5;
	EXPECT_EQ(calculateMaterialScore(&board, white), -640);
	EXPECT_EQ(calculateMaterialScore(&board, black), 640);

	board.blackKnightBitboard = 0;
	board.whiteQueenBitboard = 8;
	EXPECT_EQ(calculateMaterialScore(&board, white), 900);
	EXPECT_EQ(calculateMaterialScore(&board, black), -900);

	board.defaults();
	EXPECT_EQ(calculateMaterialScore(&board, black), 0);
	EXPECT_EQ(calculateMaterialScore(&board, white), 0);
}

TEST(Scoring, positionalScore)
{
	Board board;
	board.whiteRookBitboard = 1;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white), 0);
	EXPECT_EQ(calculatePositionalScore(&board, black), 0);

	board = Board();
	board.whitePawnBitboard = 512;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white), 10);
	EXPECT_EQ(calculatePositionalScore(&board, black), -10);

	board = Board();
	board.whiteKnightBitboard = 2048;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white), 5);
	EXPECT_EQ(calculatePositionalScore(&board, black), -5);

	board = Board();
	board.blackKnightBitboard = 4503599627370496;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white), -5);
	EXPECT_EQ(calculatePositionalScore(&board, black), 5);
}