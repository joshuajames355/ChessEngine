#include "scoring.h"

int calculateScoreDiff(Board* board)
{
	int positionalScore = calculatePositionalScore(board);
	return  positionalScore;
}

int calculatePositionalScore(Board* board)
{
	bool lateGame = isLateGame(board);

	int whiteScore = 0;
	whiteScore += pieceSquareData::pawnSquare.calcScore(board->whitePawnBitboard, white);
	whiteScore += pieceSquareData::knightSquare.calcScore(board->whiteKnightBitboard, white);
	whiteScore += pieceSquareData::bishopSquare.calcScore(board->whiteBishopBitboard, white);
	whiteScore += pieceSquareData::rookSquare.calcScore(board->whiteRookBitboard, white);
	whiteScore += pieceSquareData::queenSquare.calcScore(board->whiteQueenBitboard, white);
	if (!lateGame)
	{
		whiteScore += pieceSquareData::midGameKingSquare.calcScore(board->whiteKingBitboard, white);
	}
	else
	{
		whiteScore += pieceSquareData::lateGameKingSquare.calcScore(board->whiteKingBitboard, white);
	}

	int blackScore = 0;
	blackScore += pieceSquareData::pawnSquare.calcScore(board->blackPawnBitboard, black);
	blackScore += pieceSquareData::knightSquare.calcScore(board->blackKnightBitboard, black);
	blackScore += pieceSquareData::bishopSquare.calcScore(board->blackBishopBitboard, black);
	blackScore += pieceSquareData::rookSquare.calcScore(board->blackRookBitboard, black);
	blackScore += pieceSquareData::queenSquare.calcScore(board->blackQueenBitboard, black);
	if (!lateGame)
	{
		blackScore += pieceSquareData::midGameKingSquare.calcScore(board->blackKingBitboard, black);
	}
	else
	{
		blackScore += pieceSquareData::lateGameKingSquare.calcScore(board->blackKingBitboard, black);
	}

	if (board->nextColour == black)
	{
		return blackScore - whiteScore;
	}
	else
	{
		return whiteScore - blackScore;
	}
}

bool isLateGame(Board * board)
{
	bool lateGame = false;
	if (board->whiteQueenBitboard + board->blackQueenBitboard == 0)
	{
		lateGame = true;
	}
	else if (bitSum(board->whiteQueenBitboard) < 2 && bitSum(board->blackQueenBitboard) < 2) //Can not be late game if either side has two queens.
	{
		bool blackLateGame = false;
		bool whiteLateGame = false;
		if (bitSum(board->blackQueenBitboard) == 1)
		{
			int majorPiece = bitSum(board->blackRookBitboard);
			int minorPieces = bitSum(board->blackKnightBitboard) + bitSum(board->blackBishopBitboard);
			if (majorPiece == 0 && minorPieces <= 1)
			{
				blackLateGame = true;
			}
		}
		else if (bitSum(board->whiteQueenBitboard) == 1)
		{
			int majorPiece = bitSum(board->whiteRookBitboard);
			int minorPieces = bitSum(board->whiteKnightBitboard) + bitSum(board->whiteBishopBitboard);
			if (majorPiece == 0 && minorPieces <= 1)
			{
				whiteLateGame = true;
			}
		}
		if (whiteLateGame && blackLateGame || blackLateGame && bitSum(board->whiteQueenBitboard) == 0 || whiteLateGame && bitSum(board->blackQueenBitboard) == 0)
		{
			lateGame = true;
		}
	}
	return lateGame;
}
