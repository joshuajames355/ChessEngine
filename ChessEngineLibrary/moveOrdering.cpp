#include "moveOrdering.h"

int moveRatingComparisonFunc(Move move1, Move move2)
{
	return move1.moveRating > move2.moveRating;
}

void orderSearch(std::array<Move, 150>* moveList, Board * board, int arraySize, Move TTMove, bool isBestMove, killerEntry killerMoves)
{
	std::vector<Move> killerMoveVector = killerMoves.getKillerMoves();
	int MVVLVAScore;

	for (int x = 0; x < arraySize; x++)
	{
		if ((*moveList)[x] == TTMove)
		{
			(*moveList)[x].moveRating = 5000;
		}
		else
		{
			if ((*moveList)[x].moveType == queenPromotion)
				(*moveList)[x].moveRating = 4000;
			else if ((*moveList)[x].moveType == rookPromotion)
				(*moveList)[x].moveRating = 3999;
			else if ((*moveList)[x].moveType == bishopPromotion)
				(*moveList)[x].moveRating = 3998;
			else if ((*moveList)[x].moveType == knightPromotion)
				(*moveList)[x].moveRating = 3997;
			else if ((*moveList)[x].moveType == capture)
				(*moveList)[x].moveRating = 3000 + getMVVLVAScore((*moveList)[x]);
			//If the move is in the killerMoveTable
			else if (std::find(killerMoveVector.begin(), killerMoveVector.end(), (*moveList)[x]) != killerMoveVector.begin())
				(*moveList)[x].moveRating = 2500;
			else
			{
				(*moveList)[x].moveRating = 0;
			}
		}
	}
	
	std::sort(moveList->begin(), moveList->begin() + arraySize, moveRatingComparisonFunc);
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
int getMVVLVAScore(Move move)
{
	// order is[victim][attacker] , higher scores are better
	int tradeScores[6][6] = {
		{ 6, 5, 4, 3, 2, 1 },         //Pawn Captured   1
		{ 12, 11, 10, 9, 8, 7 },      //Knight Captured 2
		{ 18, 17, 16, 15, 14, 13 },   //Bishop Captured 3
		{ 24, 23, 22, 21, 20, 16 },   //Rook Captured   4 
		{ 30, 29, 28, 27, 26, 25 },   //Queen Captured  5
		{ 36, 35, 34, 33, 32, 31 } }; //King Captured   6

	return tradeScores[move.capturedPiece][move.piece];
}

bool MVVLVAComparisonFunc(Move move1, Move move2)
{
	return getMVVLVAScore(move1) > getMVVLVAScore(move2);
}

//Most valuable victim , least valuable attacker
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize)
{ 
	std::sort(moveList->begin(), moveList->begin() + arraySize, MVVLVAComparisonFunc);
}

killerEntry::killerEntry()
{
	numKillers = 3;
	killerMoves.reserve(numKillers);
}

void killerEntry::addKillerMove(Move move)
{
	if (killerMoves.size() < numKillers)
	{
		killerMoves.push_back(move);
	}
}
