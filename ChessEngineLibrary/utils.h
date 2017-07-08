#pragma once
#include "board.h"
#include "move.h"
#include "piece.h"
#include "moveGeneration.h"

#include <intrin.h>
#include <string>
#include <stdint.h>
#include <stdlib.h> 
#include <vector>

std::string notationFromMove(const Move& move);
Move moveFromNotation(std::string moveNotation, Board* board);
void loadPolygotBook(std::string filename, std::unordered_map<uint64_t, std::string>* openingBook);