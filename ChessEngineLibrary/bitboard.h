#pragma once
#include <stdint.h>
#include <algorithm>
#include <vector>

#define emptyBitboard 0
#define universalBitboard 18446744073709551615 //2**64 - 1

#define rank1 255
#define rank2 65280
#define rank3 16711680
#define rank4 4278190080
#define rank5 1095216660480
#define rank6 280375465082880
#define rank7 71776119061217280
#define rank8 18374686479671623680

#define fileA 72340172838076673
#define fileB 144680345676153346
#define fileC 289360691352306692
#define fileD 578721382704613384
#define fileE 1157442765409226768
#define fileF 2314885530818453536
#define fileG 4629771061636907072
#define fileH 9259542123273814144

int bitSum(uint64_t bitboard);
uint64_t pop(uint64_t& bitboard);
int bitScanForward(uint64_t bitboard);
std::vector<int> getSetBits(uint64_t bitboard);
uint64_t inBetween(int from, int to);
void setupBitboardUtils();