#include "search.h"

void updateEngine(searchData * data, Move bestMove, int alpha)
{
	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (uint64_t)((data->nodes) / difftime(time(NULL), data->startTime));
	std::cout << " pv " << notationFromMove(bestMove);
	std::cout << " score upperbound " << alpha << "\n";
	
}

Move startSearch(int searchDepth, Board* board, TranspositionEntry* transpositionTable)
{
	searchData data;
	data.startTime = time(0);
	data.nodes = 0;
	
	Move bestMove;
	for (int x = 1; x <= searchDepth; x++)
	{
		data.depth = x;
		bestMove = rootSearch(x, board, &data, transpositionTable);
	}
	return bestMove;
}


Move rootSearch(int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable)
{
	data->nodes++;

	int alpha = -9999;
	int beta = 9999;

	int alphaOriginal = alpha;
	Move bestMove;
	bool isBestMove = false;

	TranspositionEntry entry = transpositionTable[board->zorbistKey % TTSize];

	std::array<Move, 150> moveList;
	int arraySize = searchForMoves(board,&moveList);
	if (entry.zorbistKey == board->zorbistKey && std::find(moveList.begin(), moveList.end(), entry.bestMove) != moveList.end())
	{
		if (entry.depth >= depthLeft)
		{
			if (entry.flag == Exact)
			{
				return entry.bestMove;
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
				return entry.bestMove;
			}
		}

		isBestMove = true;
		bestMove = entry.bestMove;
	}

	orderSearch(&moveList, board, arraySize, bestMove, isBestMove);

	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		int score = -negamax(-beta, -alpha, depthLeft - 1, board, data,true, transpositionTable);
		moveList[x].undoMove(board);
		if (score >= beta)
		{
			return bestMove;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
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

	return bestMove;
}

int negamax(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet, TranspositionEntry* transpositionTable)
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

	orderSearch(&moveList, board, arraySize, bestMove, isBestMove);

	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		int score = -negamax(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable);
		moveList[x].undoMove(board);

		if (score >= beta)
		{
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

