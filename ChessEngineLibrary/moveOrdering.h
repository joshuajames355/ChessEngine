#pragma once
#include <vector>
#include <algorithm>

#include "move.h"
#include "board.h"

void orderSearch(std::array<Move, 150>* moveList, Board * board, int arraySize, Move TTMove, bool isBestMove);
int orderQuiescentSearch(std::array<Move, 150>* moveList, Board * board, int arraySize);
int getMVVLVAScore(Move move);
bool MVVLVAComparisonFunc(Move move1, Move move2);
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize);

