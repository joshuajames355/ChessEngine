#pragma once
#include <vector>
#include <algorithm>

#include "move.h"
#include "board.h"

void orderQuiescentSearch(std::vector<Move>* moveList, Board* board);
std::vector<Move> MVVLVA(std::vector<Move>* moveList, Board* board);

