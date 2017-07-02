#include "search.h"

void updateEngine(searchData * data, Move bestMove, int alpha)
{
	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (uint64_t)((data->nodes) / difftime(time(NULL), data->startTime));
	std::cout << " pv " << notationFromMove(bestMove);
	std::cout << " score upperbound " << alpha << "\n";
	
}

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer)
{
	searchData data;
	data.startTime = time(0);
	data.nodes = 0;
	
	Move bestMove;
	int currentScore = calculateScoreDiff(board);

	for (int x = 1; x <= 50; x++)
	{
		data.depth = x;
		currentScore = MTDF(x, currentScore, board, &data, transpositionTable);
		bestMove = data.bestMove;

		if (!timer->isMoreTime())
			break;
	}
	return bestMove;
}


int MTDF(int depthLeft, int firstGuess, Board* board, searchData* data, TranspositionEntry* transpositionTable)
{
	int guess = firstGuess;
	int upperBound = 30000;
	int lowerBound = -30000;
	int beta;
	while (lowerBound < upperBound)
	{
		beta = std::max(guess, lowerBound + 1);
		guess = rootSearch(beta - 1, beta, depthLeft, board, data, transpositionTable);
		if (guess < beta)
			upperBound = guess;
		else
			lowerBound = guess;
	}

	return guess;
}

int rootSearch(int alpha, int beta, int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable)
{
	data->nodes++;

	std::vector<killerEntry>* killerMoveTable = new std::vector<killerEntry>();
	killerMoveTable->resize(depthLeft);

	int alphaOriginal = alpha;
	Move bestMove;
	bool isBestMove = false;

	TranspositionEntry entry = transpositionTable[board->zorbistKey % TTSize];

	std::array<Move, 150> moveList;
	int arraySize = searchForMoves(board, &moveList);
	if (entry.zorbistKey == board->zorbistKey && std::find(moveList.begin(), moveList.end(), entry.bestMove) != moveList.end())
	{
		if (entry.depth >= depthLeft)
		{
			if (entry.flag == Exact)
			{
				return entry.score;
			}
			else if (entry.flag == lowerBound)
			{
				alpha = std::max(alpha, entry.score);
			}
			else if (entry.flag == upperBound)
			{
				beta = std::max(beta, entry.score);
			}
			if (alpha >= beta)
			{
				return entry.score;
			}
		}

		isBestMove = true;
		bestMove = entry.bestMove;
	}

	orderSearch(&moveList, board, arraySize, bestMove, isBestMove, killerEntry());

	int score;
	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		score = -negamax(-beta, -alpha, depthLeft - 1, board, data, true, transpositionTable, killerMoveTable);
		moveList[x].undoMove(board);
		if (score >= beta)
		{
			return beta;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
			data->bestMove = bestMove;
		}
		updateEngine(data, bestMove, alpha);
	}

	TranspositionEntry newEntry = TranspositionEntry();
	newEntry.score = alpha;
	if (alpha <= alphaOriginal)
	{
		newEntry.flag = upperBound;
	}
	else if (alpha >= beta)
	{
		newEntry.flag = lowerBound;
	}
	else
	{
		newEntry.flag = Exact;
	}
	newEntry.depth = depthLeft;
	newEntry.bestMove = bestMove;
	newEntry.zorbistKey = board->zorbistKey;

	transpositionTable[board->zorbistKey % TTSize] = newEntry;

	updateEngine(data, bestMove, alpha);

	return alpha;
}



int negamax(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable)
{
	if (depthLeft == 0) return quiescence(alpha, beta, 3, board, data, isQuiet);

	data->nodes++;
	int alphaOriginal = alpha;
	Move bestMove;
	bool isBestMove = false;

	TranspositionEntry entry = transpositionTable[board->zorbistKey % TTSize];

	std::array<Move,150> moveList;
	int arraySize = searchForMoves(board, &moveList);
	if (entry.zorbistKey == board->zorbistKey && std::find(moveList.begin(), moveList.end(), entry.bestMove) != moveList.end())
	{
		if (entry.depth >= depthLeft)
		{
			if (entry.flag == Exact)
			{
				return entry.score;
			}
			else if (entry.flag == lowerBound)
			{
				alpha = std::max(alpha, entry.score);
			}
			else if (entry.flag == upperBound)
			{
				beta = std::max(beta, entry.score);
			}
			if (alpha >= beta)
			{
				return entry.score;
			}
		}
		isBestMove = true;
		bestMove = entry.bestMove;
	}

	//CheckMate / StaleMate
	if (arraySize == 0)
	{
		//Checkmate
		if (board->isPieceAttacked(bitScanForward(board->findBitboard(board->nextColour, king)), board->nextColour))
		{
			//Adds the distance to the root node onto the checkmate score.
			//This is to ensure the search algorithm prioritizes the fastest checkmate
			return -25000 + (data->depth - depthLeft);
		}
		//Stalemate
		else
		{
			return 0;
		}
	}

	orderSearch(&moveList, board, arraySize, bestMove, isBestMove,(*killerMoveTable)[depthLeft]);

	int score;
	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		score = -negamax(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		moveList[x].undoMove(board);

		if (score >= beta)
		{
			if(moveList[x].capturedPiece == blank)
				(*killerMoveTable)[depthLeft].addKillerMove(moveList[x]);
			return beta;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
		}
		
	}

	TranspositionEntry newEntry = TranspositionEntry();
	newEntry.score = alpha;
	if (alpha <= alphaOriginal)
	{
		newEntry.flag = upperBound;
	}
	else if (alpha >= beta)
	{
		newEntry.flag = lowerBound;
	}
	else
	{
		newEntry.flag = Exact;
	}
	newEntry.depth = depthLeft;
	newEntry.bestMove = bestMove;
	newEntry.zorbistKey = board->zorbistKey;

	transpositionTable[board->zorbistKey % TTSize] = newEntry;
	return alpha;
}

int quiescence(int alpha, int beta, int depthLeft, Board* board, searchData * data, bool isQuiet)
{
	data->nodes++;

	if (isQuiet)
	{
		return calculateScoreDiff(board);
	}

	int score = calculateScoreDiff(board);
	if (score >= beta) return beta;
	if (alpha < score) alpha = score;
	if (depthLeft == 0) return alpha;

	std::array<Move, 150> moveList;
	int arraySize = searchForMoves(board, &moveList);
	arraySize = orderQuiescentSearch(&moveList,board, arraySize);

	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		score = -quiescence(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture);
		moveList[x].undoMove(board);
		if (score >= beta)
		{
			return beta;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
		}
	}

	return alpha;

}

