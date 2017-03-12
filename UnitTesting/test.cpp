#pragma once
#include "gtest/gtest.h"
#include "magicBitboards.h"

#include <iostream>
#include <stdint.h>

int main(int argc, char **argv) 
{
	magicBitboards temp = magicBitboards();
	temp.setupMagicBitboards();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

