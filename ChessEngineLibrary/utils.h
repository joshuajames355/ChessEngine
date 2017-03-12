#pragma once
#include "board.h"
#include "move.h"
#include "piece.h"

#include <string>
#include <stdint.h>

std::string notationFromMove(Move& move);
Move moveFromNotation(std::string moveNotation, Board* board);