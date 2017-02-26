#include "stdafx.h"
#include "scoring.h"

int calculateScoreDiff(std::array<std::array<piece, 8>, 8> board, colours aiColour)
{
	counter pieceCounts = counter(board);

	int materialScore = calculateMaterialScore(board, aiColour, pieceCounts);
	int positionalScore = calculatePositionalScore(board, aiColour, pieceCounts);
	return materialScore + positionalScore;
}

int calculateMaterialScore(std::array<std::array<piece, 8>, 8> board, colours aiColour, counter pieceCounts)
{
	int blackScore = 0;
	blackScore += pieceCounts.WP * 100;
	blackScore += pieceCounts.BN * 320;
	blackScore += pieceCounts.BB * 330;
	blackScore += pieceCounts.BR * 500;
	blackScore += pieceCounts.BQ * 900;
	blackScore += pieceCounts.BK * 20000;

	int whiteScore = 0;
	whiteScore += pieceCounts.WP * 100;
	whiteScore += pieceCounts.WN * 320;
	whiteScore += pieceCounts.WB * 330;
	whiteScore += pieceCounts.WR * 500;
	whiteScore += pieceCounts.WQ * 900;
	whiteScore += pieceCounts.WK * 20000;

	if (aiColour == black)
	{
		return blackScore - whiteScore;
	}
	else
	{
		return whiteScore - blackScore;
	}
}

int calculatePositionalScore(std::array<std::array<piece, 8>, 8> board, colours aiColour, counter pieceCounts)
{
	bool lateGame = false;
	if (pieceCounts.WQ + pieceCounts.BQ == 0)
	{
		lateGame = true;
	}
	else if (pieceCounts.WQ < 2 && pieceCounts.BQ < 2) //Can not be late game if either side has two queens.
	{
		bool blackLateGame = false;
		bool whiteLateGame = false;
		if (pieceCounts.BQ == 1)
		{
			int majorPiece = pieceCounts.BR;
			int minorPieces = pieceCounts.BN + pieceCounts.BB;
			if (majorPiece == 0 && minorPieces <= 1)
			{
				blackLateGame = true;
			}
		}
		else if (pieceCounts.WQ == 1)
		{
			int majorPiece = pieceCounts.WR;
			int minorPieces = pieceCounts.WN + pieceCounts.WB;
			if (majorPiece == 0 && minorPieces <= 1)
			{
				whiteLateGame = true;
			}
		}
		if (whiteLateGame && blackLateGame || blackLateGame && pieceCounts.WQ == 0 || whiteLateGame && pieceCounts.BQ == 0)
		{
			lateGame = true;
		}
	}

	int whiteScore = 0;
	whiteScore += pieceSquareData::pawnSquare.calcScore(board, white);
	whiteScore += pieceSquareData::knightSquare.calcScore(board, white);
	whiteScore += pieceSquareData::bishopSquare.calcScore(board, white);
	whiteScore += pieceSquareData::rookSquare.calcScore(board, white);
	whiteScore += pieceSquareData::queenSquare.calcScore(board, white);
	if (!lateGame)
	{
		whiteScore += pieceSquareData::midGameKingSquare.calcScore(board, white);
	}
	else
	{
		whiteScore += pieceSquareData::lateGameKingSquare.calcScore(board, white);
	}

	int blackScore = 0;
	blackScore += pieceSquareData::pawnSquare.calcScore(board, black);
	blackScore += pieceSquareData::knightSquare.calcScore(board, black);
	blackScore += pieceSquareData::bishopSquare.calcScore(board, black);
	blackScore += pieceSquareData::rookSquare.calcScore(board, black);
	blackScore += pieceSquareData::queenSquare.calcScore(board, black);
	if (!lateGame)
	{
		blackScore += pieceSquareData::midGameKingSquare.calcScore(board, black);
	}
	else
	{
		blackScore += pieceSquareData::lateGameKingSquare.calcScore(board, black);
	}

	if (aiColour == black)
	{
		return blackScore - whiteScore;
	}
	else
	{
		return whiteScore - blackScore;
	}
}

counter::counter(std::array<std::array<piece, 8>, 8> board)
{

	WP = 0;
	WN = 0;
	WB = 0;
	WR = 0;
	WQ = 0;
	WK = 0;
	BP = 0;
	BN = 0;
	BB = 0;
	BR = 0;
	BQ = 0;
	BK = 0;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board[x][y].colour == white)
			{
				switch (board[x][y].type)
				{
				case pawn:
					WP += 1;
					break;

				case knight:
					WN += 1;
					break;

				case bishop:
					WB += 1;
					break;

				case rook:
					WR += 1;
					break;

				case queen:
					WQ += 1;
					break;

				case king:
					WK += 1;
					break;
				}
			}
			else
			{
				switch (board[x][y].type)
				{
				case pawn:
					BP += 1;
					break;

				case knight:
					BN += 1;
					break;

				case bishop:
					BB += 1;
					break;

				case rook:
					BR += 1;
					break;

				case queen:
					BQ += 1;
					break;

				case king:
					BK += 1;
					break;
				}
			}
		}
	}
}