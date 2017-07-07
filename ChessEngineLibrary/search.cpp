#include "search.h"

void updateEngine(searchData * data, Move* bestMove, int alpha)
{
	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (uint64_t)((data->nodes) / difftime(time(NULL), data->startTime));
	std::cout << " pv " << notationFromMove(*bestMove);
	std::cout << " score upperbound " << alpha << "\n";
	
}

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer)
{
	searchData data;
	data.startTime = time(0);
	data.nodes = 0;
	
	Move bestMove;
	for (int x = 1; x <= 50; x++)
	{
		data.depth = x;
		bestMove = rootSearch(x, board, &data, transpositionTable);

		if (!timer->isMoreTime())
			break;
	}
	return bestMove;
}


Move rootSearch(int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable)
{
	data->nodes++;

	int alpha = -30000;
	int beta = 30000;

	std::vector<killerEntry>* killerMoveTable = new std::vector<killerEntry>();
	killerMoveTable->resize(depthLeft);

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
				delete killerMoveTable;
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
				delete killerMoveTable;
				return entry.bestMove;
			}
		}

		isBestMove = true;
		bestMove = entry.bestMove;
	}

	orderSearch(&moveList, board, arraySize, &bestMove, isBestMove, killerEntry());

	int score;
	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		if (x == 0)
		{
			score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
		//Late Move Reductions
		else if(x > 4 && depthLeft >= 3 &&
						 moveList[x].moveType != capture &&
						 moveList[x].moveType != queenPromotion &&
						 moveList[x].moveType != rookPromotion &&
						 moveList[x].moveType != knightPromotion &&
						 moveList[x].moveType != bishopPromotion)
		{
			//Try a null window search at a reduced depth
			score = -negascout(-alpha - 1, -alpha, depthLeft - 2, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
		else
		{
			//Try a null window search
			score = -negascout(-alpha - 1, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
		moveList[x].undoMove(board);
		if (score >= beta)
		{
			delete killerMoveTable;
			return bestMove;   //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
		}
		updateEngine(data, &bestMove, alpha);
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

	updateEngine(data, &bestMove, alpha);

	delete killerMoveTable;
	return bestMove;
}

int negascout(int alpha, int beta, int depthLeft, Board* board, searchData* data, bool isQuiet, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable)
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

	//Threefold repetition 
	if (std::count(board->moveHistory.begin(), board->moveHistory.end(), board->moveHistory.back()) >= 3)
		return 0;

	orderSearch(&moveList, board, arraySize, &bestMove, isBestMove,(*killerMoveTable)[depthLeft]);

	int score;
	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);

		if (x == 0)
		{
			score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
		//Late Move Reductions
		else if (x > 4 && depthLeft >= 3 &&
			moveList[x].moveType != capture &&
			moveList[x].moveType != queenPromotion &&
			moveList[x].moveType != rookPromotion &&
			moveList[x].moveType != knightPromotion &&
			moveList[x].moveType != bishopPromotion)
		{
			//Try a null window search at a reduced depth
			score = -negascout(-alpha - 1, -alpha, depthLeft - 2, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
		else
		{
			//Try a null window search
			score = -negascout(-alpha - 1, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, moveList[x].moveType != capture, transpositionTable, killerMoveTable);
		}
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

