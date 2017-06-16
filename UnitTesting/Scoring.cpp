#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"

TEST(Scoring, material)
{
	Board board;
	board.whitePawnBitboard = 1;
	board.nextColour = white;
	EXPECT_EQ(calculateMaterialScore(&board), 100);
	board.nextColour = black;
	EXPECT_EQ(calculateMaterialScore(&board), -100);

	board.whitePawnBitboard = 0;
	board.blackKnightBitboard = 5;
	board.nextColour = white;
	EXPECT_EQ(calculateMaterialScore(&board), -640);
	board.nextColour = black;
	EXPECT_EQ(calculateMaterialScore(&board), 640);

	board.blackKnightBitboard = 0;
	board.whiteQueenBitboard = 8;
	board.nextColour = white;
	EXPECT_EQ(calculateMaterialScore(&board), 900);
	board.nextColour = black;
	EXPECT_EQ(calculateMaterialScore(&board), -900);

	board.defaults();
	board.nextColour = black;
	EXPECT_EQ(calculateMaterialScore(&board), 0);
	board.nextColour = white;
	EXPECT_EQ(calculateMaterialScore(&board), 0);
}

TEST(Scoring, positionalScore)
{
	Board board;
	board.whiteRookBitboard = 1;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 0);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), 0);

	board = Board();
	board.whitePawnBitboard = 512;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 10);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), -10);

	board = Board();
	board.whiteKnightBitboard = 2048;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), 5);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), -5);

	board = Board();
	board.blackKnightBitboard = 4503599627370496;
	board.update();
	board.nextColour = white;
	EXPECT_EQ(calculatePositionalScore(&board), -5);
	board.nextColour = black;
	EXPECT_EQ(calculatePositionalScore(&board), 5);
}