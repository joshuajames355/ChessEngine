#pragma once
#include <vector>
#include <algorithm>

#include "move.h"
#include "board.h"

class killerEntry
{
public:
	killerEntry();
	std::vector<Move> getKillerMoves() { return killerMoves; };
	void addKillerMove(Move move);

private:
	std::vector<Move> killerMoves;
	int numKillers;
};

void orderSearch(std::array<Move, 150>* moveList, Board * board, int arraySize, Move TTMove, bool isBestMove, killerEntry killerMoves);
int orderQuiescentSearch(std::array<Move, 150>* moveList, Board * board, int arraySize);
int getMVVLVAScore(Move move);
bool MVVLVAComparisonFunc(Move move1, Move move2);
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize);


