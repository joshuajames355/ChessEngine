#pragma once
#include <stdint.h>

extern uint64_t knightMovesArray[64];
extern uint64_t kingMovesArray[64];
extern uint64_t pawnWhiteAttacksArray[64];
extern uint64_t pawnBlackAttacksArray[64];

void setupMoveGen();