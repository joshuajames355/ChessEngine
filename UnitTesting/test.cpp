#pragma once

#include "gtest/gtest.h"
#include "board.h"

#include <iostream>
#include <stdint.h>


TEST(BoardTest, UpdateFunction)
{
	Board board;
	board.whitePawnBitboard = 2;
	board.whiteBishopBitboard = 4;
	board.update();
	EXPECT_EQ(board.allPieces, (int64_t)6);

	
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

