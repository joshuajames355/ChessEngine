#pragma once
#include "board.h"

#include <stdint.h>
#include <array>
#include <vector>
#include <unordered_map>

class magicBitboards
{
public:
	magicBitboards();


	std::array<uint64_t, 64> bishopMask;
	std::array<uint64_t, 64> rookMask;

	std::array<int, 64> magicNumberShiftBishop;
	std::array<int, 64> magicNumberShiftRook;

	std::array<uint64_t, 64> magicNumberRook;
	std::array<uint64_t, 64> magicNumberBishop;

	std::array<std::unordered_map<int, uint64_t>, 64> magicMovesRook;
	std::array<std::unordered_map<int, uint64_t>, 64> magicMovesBishop;

	void generateMagicMovesRook();
	void generateMagicMovesBishop();
};

