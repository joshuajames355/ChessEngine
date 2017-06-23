#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"

TEST(Scoring, positionalScore)
{
	Board board;
	board.whiteRookBitboard = 1;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 0 + 500);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), 0 - 500);

	board = Board();
	board.whitePawnBitboard = 512;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 10 + 100);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), -10 - 100);

	board = Board();
	board.whiteKnightBitboard = 2048;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 5 + 320);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), -5 - 320);

	board = Board();
	board.blackKnightBitboard = 4503599627370496;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), -5 - 320);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), 5 + 320);
}