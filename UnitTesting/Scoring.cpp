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
/*
TEST(Scoring, kingStructureScore)
{
	Board board;
	board.loadFromFen("k7/8/8/8/8/4P3/2PP4/3K4 b - - 0 1 ");
	EXPECT_EQ(calculateKingSafetyScore(&board), 0);

	board.loadFromFen("k7/8/8/4p3/8/8/2PP4/3K4 b - - 0 1");
	EXPECT_EQ(calculateKingSafetyScore(&board), 0);

	board.loadFromFen("k7/8/8/8/5P2/6P1/8/6K1 b - - 0 1 ");
	EXPECT_EQ(calculateKingSafetyScore(&board), 0);
}*/