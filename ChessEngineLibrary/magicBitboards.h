#pragma once
#include "board.h"

#include <stdint.h>
#include <array>
#include <vector>
#include <unordered_map>

class magicBitboards
{
public:
	static std::array<uint64_t, 64> bishopMask;
	static std::array<uint64_t, 64> rookMask;

	static std::array<int, 64> magicNumberShiftBishop;
	static std::array<int, 64> magicNumberShiftRook;

	static std::array<uint64_t, 64> magicNumberRook;
	static std::array<uint64_t, 64> magicNumberBishop;

	static std::array<std::unordered_map<int, uint64_t>, 64> magicMovesRook;
	static std::array<std::unordered_map<int, uint64_t>, 64> magicMovesBishop;

	static void setupMagicBitboards();

private:
	static void generateMagicMovesRook();
	static void generateMagicMovesBishop();
};

