#pragma once
#include "gtest/gtest.h"
#include "magicBitboards.h"
#include "bitboard.h"
#include "moveGeneration.h"

#include <iostream>
#include <stdint.h>

int main(int argc, char **argv) 
{
	magicBitboards temp = magicBitboards();
	temp.setupMagicBitboards();
	setupBitboardUtils();
	setupMoveGen();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

