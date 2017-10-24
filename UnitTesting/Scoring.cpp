#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"

TEST(Scoring, pawnStructureScore)
{
	Board board;
	board.loadFromFen("8/8/8/8/1P6/1P6/8/8 w - - 0 1");
	EXPECT_EQ(calculatePawnStructureScore(&board), 50 - 0);

	board.loadFromFen("8/5p2/5p2/8/8/8/8/8 b - - 0 1");
	EXPECT_EQ(calculatePawnStructureScore(&board), 10 - 0);

	board.loadFromFen("8/8/1P6/1p6/8/2P5/8/8 w - - 0 1 ");
	EXPECT_EQ(calculatePawnStructureScore(&board), 92 + 20);

	board.loadFromFen("8/8/8/8/8/5P2/4Pp2/8 b - - 0 1 ");
	EXPECT_EQ(calculatePawnStructureScore(&board), 100 -52);

	board.loadFromFen("8/8/8/8/8/5Pp1/4Pp2/8 w - - 0 1");
	EXPECT_EQ(calculatePawnStructureScore(&board), 52 - 212);
}

//Need to be updated for midgame/lategame transition
TEST(Scoring, kingStructureScore)
{
	Board board;
	board.loadFromFen("k7/8/8/8/8/4P3/2PP4/3K4 b - - 0 1 ");
	EXPECT_EQ(calculateKingSafetyScore(&board), -92 - 0);

	board.loadFromFen("k7/8/8/4p3/8/8/2PP4/3K4 b - - 0 1");
	EXPECT_EQ(calculateKingSafetyScore(&board), -92 -0);

	board.loadFromFen("k7/8/8/8/5P2/6P1/8/6K1 b - - 0 1 ");
	EXPECT_EQ(calculateKingSafetyScore(&board), -100 + 82);

	board.loadFromFen("k7/ppp5/1PP5/8/7p/6pP/5PP1/6K1 b - - 0 1 ");
	EXPECT_EQ(calculateKingSafetyScore(&board), -30 +32);
}

TEST(Scoring, rookPositionalScore)
{
	Board board;
	board.loadFromFen("k1r1r3/8/8/8/8/8/2P2P2/5R1K w - - 0 1");
	EXPECT_EQ(calculateRookPositionScore(&board), 0 - 25);

	board.loadFromFen("k3r3/5pR1/8/8/8/8/8/5R1K w - - 0 1");
	EXPECT_EQ(calculateRookPositionScore(&board), 45 - 15);
}

TEST(Scoring, calculateScoreDiff)
{
	Board board = Board("5rk1/pp2npp1/2p1r2p/2qpP2P/P3P3/2B2PQ1/2P2P2/3RR1K1 w - - 1 28");

	EXPECT_EQ(calculatePawnStructureScore(&board), -96);
	EXPECT_EQ(calculateKingSafetyScore(&board), -24);

}