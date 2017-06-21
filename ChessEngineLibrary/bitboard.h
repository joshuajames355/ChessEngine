#pragma once
#include <stdint.h>
#include <vector>

int bitSum(uint64_t bitboard);
uint64_t pop(uint64_t& bitboard);
int bitScanForward(uint64_t bitboard);
std::vector<int> getSetBits(uint64_t bitboard);