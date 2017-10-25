#include "search.h"

void updateUI(searchData * data, Move currentMove, int currentMoveNumber, std::string pvLine)
{
	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (uint64_t)((data->nodes) / difftime(time(NULL), data->startTime));
	std::cout << " pv " << pvLine;
	std::cout << " currmove " << notationFromMove(currentMove);
	std::cout << " currmovenumber " << currentMoveNumber << "\n";
}

void finalUIUpdate(searchData * data, std::string pvLine)
{
	std::cout << "info depth " << data->depth;
	std::cout << " nodes " << data->nodes;
	std::cout << " nps " << (uint64_t)((data->nodes) / difftime(time(NULL), data->startTime));
	std::cout << " pv " << pvLine << "\n";
}

Move startSearch(Board* board, TranspositionEntry* transpositionTable, timeManagement* timer)
{
	searchData data;
	data.startTime = time(0);
	data.nodes = 0;
	
	PVData bestMove;
	for (int x = 1; x <= 50; x++)
	{
		data.depth = x;

		std::vector<killerEntry>* killerMoveTable = new std::vector<killerEntry>();
		killerMoveTable->resize(x + 1);

		negascout(-30000, 30000, x, board, &data, transpositionTable, killerMoveTable);

		delete killerMoveTable;

		bestMove = extractPVLine(board, transpositionTable, x);

		if (!timer->isMoreTime(x))
			break;
	}
	finalUIUpdate(&data, bestMove.line);
	return bestMove.bestMove;
}

int negascout(int alpha, int beta, int depthLeft, Board* board, searchData* data, TranspositionEntry* transpositionTable, std::vector<killerEntry>* killerMoveTable)
{
	if (depthLeft == 0) return quiescence(alpha, beta, 3, board, data, false);

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
		if (board->isInCheck())
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
	if (board->moveHistory.size() > 0)
	{
		if (std::count(board->moveHistory.begin(), board->moveHistory.end(), board->moveHistory.back()) >= 3)
			return 0;
	}

	orderSearch(&moveList, board, arraySize, &bestMove, isBestMove,(*killerMoveTable)[depthLeft]);

	int score;
	for (int x = 0; x < arraySize; x++)
	{
		//If First node
		if (depthLeft == data->depth)
		{
			PVData bestMove = extractPVLine(board, transpositionTable, depthLeft);
			updateUI(data, moveList[x], x + 1, bestMove.line);
		}

		moveList[x].applyMove(board);
		if (x == 0)
		{
			score = -negascout(-beta, -alpha, depthLeft - 1, board, data, transpositionTable, killerMoveTable);
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
			score = -negascout(-alpha - 1, -alpha, depthLeft - 2, board, data, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, transpositionTable, killerMoveTable);
		}
		else
		{
			//Try a null window search
			score = -negascout(-alpha - 1, -alpha, depthLeft - 1, board, data, transpositionTable, killerMoveTable);

			//If the score is within the bounds , the first child was not the principle variation
			if (alpha < score && score < beta)
				//Therefore do a full re-search
				score = -negascout(-beta, -alpha, depthLeft - 1, board, data, transpositionTable, killerMoveTable);
		}
		moveList[x].undoMove(board);

		if (score > alpha)
		{
			alpha = score; // alpha acts like max in MiniMax
			bestMove = moveList[x];
		}
		if (score >= beta)
		{
			if(moveList[x].capturedPiece == blank)
				(*killerMoveTable)[depthLeft].addKillerMove(moveList[x]);
			break;   //  fail hard beta-cutoff
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
		score = -quiescence(-beta, -alpha, depthLeft - 1, board, data, !continueQuiescence( board, &moveList[x]));
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

bool continueQuiescence(Board * board, Move * nextMove)
{
	if (board->isInCheck()) return true;
	if (nextMove->moveType == capture || nextMove->moveType == rookPromotion ||
		nextMove->moveType == queenPromotion || bishopPromotion || queenPromotion) return true;
	return false;
}

PVData extractPVLine(Board * board, TranspositionEntry * transpositionTable, int expectedDepth)
{
	PVData pv;

	if (expectedDepth == 0) return pv;

	TranspositionEntry entry = transpositionTable[board->zorbistKey % TTSize];
	if (entry.zorbistKey == board->zorbistKey) //The move has previously been searched
	{

		std::array<Move, 150> moveList;
		int arraySize = searchForMoves(board, &moveList);

		//If the move is invalid
		if (std::find(moveList.begin(), moveList.begin() + arraySize, entry.bestMove) == moveList.begin() + arraySize)
		{
			return pv;
		}

		pv.bestMove = entry.bestMove;
		pv.line += notationFromMove(entry.bestMove) + " ";

		entry.bestMove.applyMove(board);		
		
		pv.line += extractPVLine(board, transpositionTable, expectedDepth - 1).line;
		entry.bestMove.undoMove(board);
	}

	return pv;
}

