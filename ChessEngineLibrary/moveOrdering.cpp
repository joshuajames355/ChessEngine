#include "moveOrdering.h"

void orderSearch(std::array<Move, 150>* moveList, Board * board, int arraySize, Move TTMove, bool isBestMove)
{
	if (isBestMove) 
		std::swap(moveList->begin(), std::find(moveList->begin(), moveList->begin() + arraySize, TTMove));
}

int orderQuiescentSearch(std::array<Move, 150>* moveList, Board * board, int arraySize)
{
	//Filters out non-capture moves
	std::array<Move, 150> captureMoves;
	int counter = 0;
	for (int x = 0; x < arraySize; x++)
	{
		if ((*moveList)[x].capturedPiece != blank)
		{
			captureMoves[counter] = (*moveList)[x];
			counter++;
		}
	}
	arraySize = counter;
	*moveList = captureMoves;

	MVVLVA(moveList, board, arraySize);
	
	return arraySize;
}

bool MVVLVAComparisonFunc(Move move1, Move move2)
{
	// order is[victim][attacker] , higher scores are better
	int tradeScores[6][6] = { 
	{6, 5, 4, 3, 2, 1},         //Pawn Captured   1
	{12, 11, 10, 9, 8, 7},      //Knight Captured 2
	{18, 17, 16, 15, 14, 13},   //Bishop Captured 3
	{24, 23, 22, 21, 20, 16},   //Rook Captured   4 
	{30, 29, 28, 27, 26, 25},   //Queen Captured  5
	{36, 35, 34, 33, 32, 31} }; //King Captured   6

	return tradeScores[move1.capturedPiece][move1.piece] > tradeScores[move2.capturedPiece][move2.piece];
}

//Most valuable victim , least valuable attacker
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize)
{ 
	std::sort(moveList->begin(), moveList->begin() + arraySize, MVVLVAComparisonFunc);
}
