#pragma once
#include "board.h"
#include "move.h"
#include "piece.h"
#include "moveGeneration.h"

#include <string>
#include <stdint.h>
#include <stdlib.h> 
#include <vector>

std::string notationFromMove(const Move& move);
Move moveFromNotation(std::string moveNotation, Board* board);
bool isInCheck(Board* board, colours colour);