#include "moveTree.h"

void updateEngine(searchData * data, Move bestMove, int alpha)
{

	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (data->nodes) / difftime(time(NULL), data->startTime);
	std::cout << " pv " << notationFromMove(bestMove);
	std::cout << " score upperbound " << alpha << "\n";
	
}

Move startSearch(int searchDepth, Board board, colours colour)
{
	ZorbistKeys::TranspositionTable.clear();
	Move bestMove;
	for (int x = 1; x <= searchDepth; x++)
	{
		bestMove = rootSearch(x, board, colour);
	}
	bestMove = rootSearch(searchDepth, board, colour);
	return bestMove;
}


Move rootSearch(int depthLeft, Board board, colours colour)
{
	searchData data;
	data.startTime = time(0);
	data.nodes = 1;
	data.depth = depthLeft;

	int alpha = -9999;
	int beta = 9999;


	int alphaOriginal = alpha;
	Move bestMove;
	bool isBestMove = false;
	int hash = getZorbistKey(&board, colour);
	if (ZorbistKeys::TranspositionTable.find(hash) != ZorbistKeys::TranspositionTable.end())
	{
		TranspositionEntry entry = ZorbistKeys::TranspositionTable[hash];
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
		else
		{
			std::cout << "move loaded \n";
			isBestMove = true;
			bestMove = entry.bestMove;
		}
	}

	std::vector<Move> moveList = searchForMoves(&board, colour);
	if (isBestMove)
	{
		//Moves bestMove to the front of the moveList.
		//moveList.erase(std::remove(moveList.begin(), moveList.end(), bestMove), moveList.end());
		moveList.insert(moveList.begin(), bestMove);
	}
	colours newColour = switchColour(colour);
	Board newBoard;
	int newHash;
	for (int x = 0; x < moveList.size(); x++)
	{
		if (isBestMove && x != 0) {
			if (moveList[x].from == bestMove.from && moveList[x].to == bestMove.to) continue;
		}
		newBoard = moveList[x].applyMove(&board, colour);
		if (moveList[x].moveType == quietMove)
		{
			newHash = updateHash(moveList[x], hash, newColour);
		}
		else
		{
			newHash = getZorbistKey(&newBoard, newColour);
		}
		int score = -negamax(-beta, -alpha, depthLeft - 1, newBoard, newColour, &data, newHash,true);
		if (score >= beta)
		{
			return bestMove;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
		}
		updateEngine(&data, bestMove, alpha);

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

	ZorbistKeys::TranspositionTable[hash] = newEntry;

	updateEngine(&data, bestMove, alpha);

	return bestMove;
}

int negamax(int alpha, int beta, int depthLeft, Board board, colours colour, searchData* data, int hash, bool isQuiet)
{
	data->nodes++;
	int alphaOriginal = alpha;
	Move bestMove;
	bool isBestMove = false;
	if (ZorbistKeys::TranspositionTable.find(hash) != ZorbistKeys::TranspositionTable.end())
	{
		TranspositionEntry entry = ZorbistKeys::TranspositionTable[hash];
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
		else
		{
			isBestMove = true;
			bestMove = entry.bestMove;
		}
	}


	if (depthLeft <= 0 && isQuiet)
	{
		return calculateScoreDiff(&board, colour);
	}
	else if (depthLeft <= 0)
	{
		int score = calculateScoreDiff(&board, colour);
		if (score >= beta)
			return beta;
		if (alpha < score)
			alpha = score;
		if (depthLeft <= -4) return alpha; //Limits Quiescence Search to 4 ply/

	}

	std::vector<Move> moveList = searchForMoves(&board, colour);
	if (isBestMove)
	{
		//Moves bestMove to the front of the moveList.
		//moveList.erase(std::remove(moveList.begin(), moveList.end(), bestMove), moveList.end());
		moveList.insert(moveList.begin(), bestMove);
	}
	colours newColour = switchColour(colour);
	Board newBoard;
	int newHash;
	for (int x = 0; x < moveList.size(); x++)
	{
		if (depthLeft <= 0 && moveList[x].moveType != capture) continue;
		if (isBestMove && x != 0) {
			if (moveList[x].from == bestMove.from && moveList[x].to == bestMove.to) continue; 
		}
		newBoard = moveList[x].applyMove(&board, colour);
		if (moveList[x].moveType == quietMove)
		{
			newHash = updateHash(moveList[x], hash, newColour);
		}
		else
		{
			newHash = getZorbistKey(&newBoard, newColour);
		}
		int score = -negamax(-beta, -alpha, depthLeft - 1, newBoard, newColour, data, newHash, moveList[x].moveType != capture);
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

	ZorbistKeys::TranspositionTable[hash] = newEntry;
	return alpha;
}



