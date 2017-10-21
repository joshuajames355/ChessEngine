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
	whiteScore += pieceSquareData::pawnSquare.calcScore(board->getPieceBitboard(white, pawn), white);
	whiteScore += pieceSquareData::knightSquare.calcScore(board->getPieceBitboard(white, king), white);
	whiteScore += pieceSquareData::bishopSquare.calcScore(board->getPieceBitboard(white, bishop), white);
	whiteScore += pieceSquareData::rookSquare.calcScore(board->getPieceBitboard(white, rook), white);
	whiteScore += pieceSquareData::queenSquare.calcScore(board->getPieceBitboard(white, queen), white);
	if (!lateGame)
	{
		whiteScore += pieceSquareData::midGameKingSquare.calcScore(board->getPieceBitboard(white, king), white);
	}
	else
	{
		whiteScore += pieceSquareData::lateGameKingSquare.calcScore(board->getPieceBitboard(white, king), white);
	}

	int blackScore = 0;
	blackScore += pieceSquareData::pawnSquare.calcScore(board->getPieceBitboard(black, pawn), black);
	blackScore += pieceSquareData::knightSquare.calcScore(board->getPieceBitboard(black, knight) , black);
	blackScore += pieceSquareData::bishopSquare.calcScore(board->getPieceBitboard(black, bishop), black);
	blackScore += pieceSquareData::rookSquare.calcScore(board->getPieceBitboard(black, rook), black);
	blackScore += pieceSquareData::queenSquare.calcScore(board->getPieceBitboard(black, queen), black);
	if (!lateGame)
	{
		blackScore += pieceSquareData::midGameKingSquare.calcScore(board->getPieceBitboard(black, king), black);
	}
	else
	{
		blackScore += pieceSquareData::lateGameKingSquare.calcScore(board->getPieceBitboard(black, king), black);
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
	if (board->getPieceBitboard(white, queen) + board->getPieceBitboard(black, queen) == 0)
	{
		lateGame = true;
	}
	else if (bitSum(board->getPieceBitboard(white, queen)) < 2 && bitSum(board->getPieceBitboard(black, queen)) < 2) //Can not be late game if either side has two queens.
	{
		bool blackLateGame = false;
		bool whiteLateGame = false;
		if (bitSum(board->getPieceBitboard(black, queen)) == 1)
		{
			int majorPiece = bitSum(board->getPieceBitboard(black, rook));
			int minorPieces = bitSum(board->getPieceBitboard(black, knight)) + bitSum(board->getPieceBitboard(black, bishop));
			if (majorPiece == 0 && minorPieces <= 1)
			{
				blackLateGame = true;
			}
		}
		else if (bitSum(board->getPieceBitboard(white, queen)) == 1)
		{
			int majorPiece = bitSum(board->getPieceBitboard(white, rook));
			int minorPieces = bitSum(board->getPieceBitboard(white, knight)) + bitSum(board->getPieceBitboard(white, bishop));
			if (majorPiece == 0 && minorPieces <= 1)
			{
				whiteLateGame = true;
			}
		}
		if (whiteLateGame && blackLateGame || blackLateGame && bitSum(board->getPieceBitboard(white, queen)) == 0 || whiteLateGame && bitSum(board->getPieceBitboard(black, queen)) == 0)
		{
			lateGame = true;
		}
	}
	return lateGame;
}
