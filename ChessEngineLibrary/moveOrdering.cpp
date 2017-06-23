#include "moveOrdering.h"

void orderQuiescentSearch(std::vector<Move>* moveList, Board * board)
{
	*moveList = MVVLVA(moveList, board);
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
std::vector<Move> MVVLVA(std::vector<Move>* moveList, Board * board)
{ 
	std::vector<Move> captureMoves;
	captureMoves.reserve(moveList->size());
	for (int x = 0; x < moveList->size(); x++)
	{
		if ((*moveList)[x].moveType == capture)
		{
			captureMoves.push_back((*moveList)[x]);
		}
	}
	std::sort(captureMoves.begin(), captureMoves.end(), MVVLVAComparisonFunc);

	return captureMoves;
}
