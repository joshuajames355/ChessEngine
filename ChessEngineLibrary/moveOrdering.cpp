#include "moveOrdering.h"

int moveRatingComparisonFunc(Move move1, Move move2)
{
	return move1.moveRating > move2.moveRating;
}

void orderSearch(std::array<Move, 150>* moveList, Board* board, int arraySize, Move* TTMove, bool isBestMove, killerEntry killerMoves, std::array<std::array<std::array<Move, 64>, 64>, 2>* counterMoves, Move* prevMove, std::array<std::array<std::array<int, 64>, 64>, 2>* historyMoves)
{
	std::vector<Move> killerMoveVector = killerMoves.getKillerMoves();
	int MVVLVAScore;
	int seeScore;

	//Move order scheme:
	//1. Move from hash table (score 5000)
	//2. Promotions (Score ~ 4000)
	//3. Winning or equal captures (SEE >= 0) , (Score 3000 + SEE)
	//4. Strong non-captures (killer move , then countermove heuristic), (Score 2500, 2400)
	//5. Other non-captures sorted by history heuristic, (Score 2000 + history)
	//6. Losing captures, ( Score 2000 - SEE penalty)

	for (int x = 0; x < arraySize; x++)
	{
		if ((*moveList)[x] == *TTMove)
		{
			(*moveList)[x].moveRating = 5000;
		}
		else
		{
			if ((*moveList)[x].capturedPiece != blank)
			{
				seeScore = SEE(&(*moveList)[x], board);
			}

			if ((*moveList)[x].moveType == queenPromotion)
				(*moveList)[x].moveRating = 4000;
			else if ((*moveList)[x].moveType == rookPromotion)
				(*moveList)[x].moveRating = 3999;
			else if ((*moveList)[x].moveType == bishopPromotion)
				(*moveList)[x].moveRating = 3998;
			else if ((*moveList)[x].moveType == knightPromotion)
				(*moveList)[x].moveRating = 3997;
			else if ((*moveList)[x].capturedPiece != blank && seeScore >= 0)
				(*moveList)[x].moveRating = 3000 + seeScore;
			//If the move is in the killerMoveTable
			else if (std::find(killerMoveVector.begin(), killerMoveVector.end(), (*moveList)[x]) != killerMoveVector.begin())
				(*moveList)[x].moveRating = 2500;
			//If the move is in the countermove table
			else if (prevMove != nullptr && (*moveList)[x] == (*counterMoves)[board->nextColour][prevMove->from][prevMove->to])
						(*moveList)[x].moveRating = 2400;
			//If the move is in the history table
			else if ((*moveList)[x].capturedPiece != blank)
			{
				(*moveList)[x].moveRating = 2000 + seeScore;
			}
			else
			{
				(*moveList)[x].moveRating = 2000 + (*historyMoves)[board->nextColour][(*moveList)[x].from][(*moveList)[x].to];
			}
		}
	}
	
	std::sort(moveList->begin(), moveList->begin() + arraySize, moveRatingComparisonFunc);
}

int orderQuiescentSearch(std::array<Move, 150>* moveList, Board * board, int arraySize)
{
	//Filters out non-capture moves
	int counter = 0;
	for (int x = 0; x < arraySize; x++)
	{
		if ((*moveList)[x].capturedPiece != blank)
		{
			if (counter != x) (*moveList)[counter] = (*moveList)[x];
			counter++;
		}
	}
	arraySize = counter;

	const int materialValues[6] = { 100,300,300,500,900,0 };

	for (int x = 0; x < arraySize; x++)
	{
		//When weaker pieces capture stronger pieces, the move is probably strong , no need to waste time in SEE
		if (materialValues[(*moveList)[x].piece] < materialValues[(*moveList)[x].capturedPiece])
			(*moveList)[x].moveRating = materialValues[(*moveList)[x].capturedPiece];
		else
			(*moveList)[x].moveRating = SEE(&(*moveList)[x], board);
	}
	
	std::sort(moveList->begin(), moveList->begin() + arraySize, moveRatingComparisonFunc);

	return arraySize;
}
int getMVVLVAScore(Move* move)
{
	// order is[victim][attacker] , higher scores are better
	int tradeScores[6][6] = {
		{ 6, 5, 4, 3, 2, 1 },         //Pawn Captured   1
		{ 12, 11, 10, 9, 8, 7 },      //Knight Captured 2
		{ 18, 17, 16, 15, 14, 13 },   //Bishop Captured 3
		{ 24, 23, 22, 21, 20, 19 },   //Rook Captured   4 
		{ 30, 29, 28, 27, 26, 25 },   //Queen Captured  5
		{ 36, 35, 34, 33, 32, 31 } }; //King Captured   6

	return tradeScores[move->capturedPiece][move->piece];
}

bool MVVLVAComparisonFunc(Move move1, Move move2)
{
	return getMVVLVAScore(&move1) > getMVVLVAScore(&move2);
}

//Most valuable victim , least valuable attacker
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize)
{ 
	std::sort(moveList->begin(), moveList->begin() + arraySize, MVVLVAComparisonFunc);
}

int SEE(Move * move, Board * board)
{
	if (move->capturedPiece == blank) return 0;

	const int materialValues[6] = { 100,300,300,500,900,0 };

	//Keeps track of all pieces (other than those that we have considered moved or captured)
	uint64_t allPieces = board->allPieces & ~((uint64_t)1 << move->from);

	std::array<SEEPiece,20> whiteAttackers;
	std::array<SEEPiece, 20> blackAttackers;
	int whiteAttackersArraySize = 0;
	int blackAttackersArraySize = 0;

	//The total score off all piece captured
	int score = materialValues[move->capturedPiece];
	//The value of the current piece in the target square
	int tempScore = materialValues[move->piece];

	colours currentColour = switchColour(board->nextColour);

	//Adds all the piece that can attack the target square
	uint64_t whitePawnBitboard = pawnBlackAttacksArray[move->to] & board->getPieceBitboard(white, pawn) & allPieces;
	while (whitePawnBitboard)
	{
		whiteAttackers[whiteAttackersArraySize] = SEEPiece(pop(whitePawnBitboard), pawn);
		whiteAttackersArraySize++;
	}

	uint64_t blackPawnBitboard = pawnWhiteAttacksArray[move->to] & board->getPieceBitboard(black, pawn) & allPieces;
	while (blackPawnBitboard)
	{
		blackAttackers[blackAttackersArraySize] = SEEPiece(pop(blackPawnBitboard), pawn);
		blackAttackersArraySize++;
	}

	uint64_t whiteKnightBitboard = knightMovesArray[move->to] & board->getPieceBitboard(white, knight) & allPieces;
	while (whiteKnightBitboard)
	{
		whiteAttackers[whiteAttackersArraySize] = SEEPiece(pop(whiteKnightBitboard), knight);
		whiteAttackersArraySize++;
	}

	uint64_t blackKnightBitboard = knightMovesArray[move->to] & board->getPieceBitboard(black, knight) & allPieces;
	while (blackKnightBitboard)
	{
		blackAttackers[blackAttackersArraySize] = SEEPiece(pop(blackKnightBitboard), knight);
		blackAttackersArraySize++;
	}

	uint64_t whiteKingBitboard = kingMovesArray[move->to] & board->getPieceBitboard(white, king) & allPieces;
	while (whiteKingBitboard)
	{
		whiteAttackers[whiteAttackersArraySize] = SEEPiece(pop(whiteKingBitboard), king);
		whiteAttackersArraySize++;
	}

	uint64_t blackKingBitboard = kingMovesArray[move->to] & board->getPieceBitboard(black, king) & allPieces;
	while (blackKingBitboard)
	{
		blackAttackers[blackAttackersArraySize] = SEEPiece(pop(blackKingBitboard), king);
		blackAttackersArraySize++;
	}

	SEEAddSliders(&whiteAttackers, &blackAttackers, board, move->to, allPieces, &whiteAttackersArraySize, &blackAttackersArraySize);

	while (true)
	{
		std::array<SEEPiece, 20>* currentAttackers = (currentColour == white) ? &whiteAttackers : &blackAttackers;
		int* currentAttackersSize = (currentColour == white) ? &whiteAttackersArraySize : &blackAttackersArraySize;

		if (*currentAttackersSize == 0) break;

		const SEEPiece smallestPiece = getLeastValuablePiece(currentAttackers, currentAttackersSize);
	
		if (currentColour == board->nextColour) score += tempScore;
		else score -= tempScore;

		tempScore = materialValues[smallestPiece.type];
		allPieces &= ~smallestPiece.pieceBitboard;
		SEEAddSliders(&whiteAttackers, &blackAttackers, board, move->to, allPieces, &whiteAttackersArraySize, &blackAttackersArraySize);

		currentColour = switchColour(currentColour);
	}

	return score;
}

//Adds any sliding pieces that can attack the target square to the respective vectors
void SEEAddSliders(std::array<SEEPiece,20>* whiteAttackers, std::array<SEEPiece,20>* blackAttackers, Board* board, int targetSquare, uint64_t occupancyMask, int* whiteAttackersArraySize, int* blackAttackersArraySize)
{
	const int pieceBitboard = (uint64_t)1 << targetSquare;

	uint64_t occupancy = magicBitboards::rookMask[targetSquare] & occupancyMask;
	uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[targetSquare];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[targetSquare];
	uint64_t horizontalSliderPositionsBitboard = magicBitboards::magicMovesRook[targetSquare][arrayIndex] & occupancyMask;

	uint64_t whiteRookPositionsBitboad = horizontalSliderPositionsBitboard & board->getPieceBitboard(white, rook);
	while(whiteRookPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(whiteRookPositionsBitboad),rook);
		if (std::find(whiteAttackers->begin(), whiteAttackers->begin() + *whiteAttackersArraySize, seePiece) == whiteAttackers->begin() + *whiteAttackersArraySize)
		{
			(*whiteAttackers)[*whiteAttackersArraySize] = seePiece;
			(*whiteAttackersArraySize)++;
		}
	}
	uint64_t blackRookPositionsBitboad = horizontalSliderPositionsBitboard & board->getPieceBitboard(black, rook);
	while (blackRookPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(blackRookPositionsBitboad), rook);
		if (std::find(blackAttackers->begin(), blackAttackers->begin() + *blackAttackersArraySize, seePiece) == blackAttackers->begin() + *blackAttackersArraySize)
		{
			(*blackAttackers)[*blackAttackersArraySize] = seePiece;
			(*blackAttackersArraySize)++;
		}
	}
	uint64_t whiteQueenPositionsBitboad = horizontalSliderPositionsBitboard & board->getPieceBitboard(white, queen);
	while (whiteQueenPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(whiteQueenPositionsBitboad), queen);
		if (std::find(whiteAttackers->begin(), whiteAttackers->begin() + *whiteAttackersArraySize, seePiece) == whiteAttackers->begin() + *whiteAttackersArraySize)
		{
			(*whiteAttackers)[*whiteAttackersArraySize] = seePiece;
			(*whiteAttackersArraySize)++;
		}
	}
	uint64_t blackQueenPositionsBitboad = horizontalSliderPositionsBitboard & board->getPieceBitboard(black, queen);
	while (blackQueenPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(blackQueenPositionsBitboad), queen);
		if (std::find(blackAttackers->begin(), blackAttackers->begin() + *blackAttackersArraySize, seePiece) == blackAttackers->begin() + *blackAttackersArraySize)
		{
			(*blackAttackers)[*blackAttackersArraySize] = seePiece;
			(*blackAttackersArraySize)++;
		}
	}

	occupancy = magicBitboards::bishopMask[targetSquare] & occupancyMask;
	magicResult = occupancy * magicBitboards::magicNumberBishop[targetSquare];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[targetSquare];
	const uint64_t diagonalSliderPositionsBitboard = magicBitboards::magicMovesBishop[targetSquare][arrayIndex] & occupancyMask;
	
	uint64_t whiteBishopPositionsBitboad = diagonalSliderPositionsBitboard& board->getPieceBitboard(white, bishop);
	while (whiteBishopPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(whiteBishopPositionsBitboad), bishop);
		if (std::find(whiteAttackers->begin(), whiteAttackers->begin() + *whiteAttackersArraySize, seePiece) == whiteAttackers->begin() + *whiteAttackersArraySize)
		{
			(*whiteAttackers)[*whiteAttackersArraySize] = seePiece;
			(*whiteAttackersArraySize)++;
		}
	}
	uint64_t blackBishopPositionsBitboad = diagonalSliderPositionsBitboard& board->getPieceBitboard(black, bishop);
	while (blackBishopPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(blackBishopPositionsBitboad), bishop);
		if (std::find(blackAttackers->begin(), blackAttackers->begin() + *blackAttackersArraySize, seePiece) == blackAttackers->begin() + *blackAttackersArraySize)
		{
			(*blackAttackers)[*blackAttackersArraySize] = seePiece;
			(*blackAttackersArraySize)++;
		}
	}
	whiteQueenPositionsBitboad = diagonalSliderPositionsBitboard& board->getPieceBitboard(white, queen);
	while (whiteQueenPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(whiteQueenPositionsBitboad), queen);
		if (std::find(whiteAttackers->begin(), whiteAttackers->begin() + *whiteAttackersArraySize, seePiece) == whiteAttackers->begin() + *whiteAttackersArraySize)
		{
			(*whiteAttackers)[*whiteAttackersArraySize] = seePiece;
			(*whiteAttackersArraySize)++;
		}
	}
	blackQueenPositionsBitboad = diagonalSliderPositionsBitboard& board->getPieceBitboard(black, queen);
	while (blackQueenPositionsBitboad)
	{
		const SEEPiece seePiece = SEEPiece(pop(blackQueenPositionsBitboad), queen);
		if (std::find(blackAttackers->begin(), blackAttackers->begin() + *blackAttackersArraySize, seePiece) == blackAttackers->begin() + *blackAttackersArraySize)
		{
			(*blackAttackers)[*blackAttackersArraySize] = seePiece;
			(*blackAttackersArraySize)++;
		}
	}
}

SEEPiece getLeastValuablePiece(std::array<SEEPiece, 20>* attackers, int* arraySize)
{
	const int materialValues[6] = { 100,300,300,500,900,0 };
	SEEPiece min;
	int minMaterial = 9999;
	int minPos;
	for (int x = 0; x < *arraySize; x++)
	{
		if (materialValues[(*attackers)[x].type] < minMaterial)
		{
			minPos = x;
			min = (*attackers)[x];
			minMaterial = materialValues[(*attackers)[x].type];
		}
	}

	(*attackers)[minPos] = (*attackers)[*arraySize - 1];
	(*arraySize)--;

	return min;
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

SEEPiece::SEEPiece(uint64_t newPieceBitboard, pieceType newPieceType)
{
	pieceBitboard = newPieceBitboard;
	type = newPieceType;
}
