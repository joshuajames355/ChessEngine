#pragma once
#include "gtest/gtest.h"
#include "moveOrdering.h"
#include "utils.h"

TEST(MoveOrdering, SEE)
{
	Board board = Board("1k1r4/1pp4p/p7/4p3/8/P5P1/1PP4P/2K1R3 w - - 0");
	Move move = moveFromNotation("e1e5", &board);
	EXPECT_EQ(SEE(&move, &board), 100);

	board = Board("1k1r3q/1ppn3p/p4b2/4p3/8/P2N2P1/1PP1R1BP/2K1Q3 w - - 0");
	move = moveFromNotation("d3e5", &board);
	EXPECT_EQ(SEE(&move, &board), -1000);

	board = Board("8/3n4/8/4N3/3P4/8/8/8 b - - 0 1 ");
	move = moveFromNotation("d7e5", &board);
	EXPECT_EQ(SEE(&move, &board), 0);
}