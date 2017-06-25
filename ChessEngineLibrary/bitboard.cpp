#include "bitboard.h"

int bitSum(uint64_t bitboard)
{
	int count = 0;
	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}
	return count;
}

uint64_t pop(uint64_t& bitboard)
{
	uint64_t lsb = bitboard & -bitboard;
	bitboard -= lsb;
	return lsb;
}

const int index64[64] = {
	0,  1, 48,  2, 57, 49, 28,  3,
	61, 58, 50, 42, 38, 29, 17,  4,
	62, 55, 59, 36, 53, 51, 43, 22,
	45, 39, 33, 30, 24, 18, 12,  5,
	63, 47, 56, 27, 60, 41, 37, 16,
	54, 35, 52, 21, 44, 32, 23, 11,
	46, 26, 40, 15, 34, 20, 31, 10,
	25, 14, 19,  9, 13,  8,  7,  6
};

int bitScanForward(uint64_t bitboard) {
	const uint64_t debruijn64 = 285870213051386505;
	return index64[((bitboard & -bitboard) * debruijn64) >> 58];
}

std::vector<int> getSetBits(uint64_t bitboard)
{
	std::vector<int> setBits;
	while (bitboard)
	{
		uint64_t pos = pop(bitboard);
		setBits.push_back(bitScanForward(pos));
	}
	return setBits;
}

uint64_t inBetweenLookup[64][64];

uint64_t inBetween(int from, int to)
{
	return inBetweenLookup[from][to];
}

void setupBitboardUtils()
{
	for (int from = 0; from < 64; from++)
	{
		for (int to = 0; to < 64; to++)
		{
			inBetweenLookup[from][to] = 0;
			if (from % 8 == to % 8) //Same column
			{
				if (to > from)
				{
					for (int x = from + 8; x < to; x += 8)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
				else
				{
					for (int x = from - 8; x > to; x -= 8)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
			}
			else if (from / 8 == to / 8) //Same row
			{
				if (to > from)
				{
					for (int x = from + 1; x < to; x += 1)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
				else
				{
					for (int x = from - 1; x > to; x -= 1)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
			}
			else if (std::abs(from - to) % 9 == 0) //Bottom-left to top-right diagonal
			{
				if (to > from)
				{
					for (int x = from + 9; x < to; x += 9)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
				else
				{
					for (int x = from - 9; x > to; x -= 9)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
			}
			else if (std::abs(from - to) % 7 == 0) //Top-left to Bottom-right diagonal
			{
				if (to > from)
				{
					for (int x = from + 7; x < to; x += 7)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
				else
				{
					for (int x = from - 7; x > to; x -= 7)
					{
						inBetweenLookup[from][to] |= (uint64_t)1 << x;
					}
				}
			}
		}
	}
}