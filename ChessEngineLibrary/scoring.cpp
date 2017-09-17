#include "scoring.h"

int calculateScoreDiff(Board* board)
{
	bool lateGame = isLateGame(board);

	int score = calculatePawnStructureScore(board);
	score += calculateMaterialScore(board, lateGame);
	score += calculateRookPositionScore(board);
	if(!isLateGame) score += calculateKingSafetyScore(board);

	return score;
}

int calculatePawnStructureScore(Board* board)
{
	int blackScore = 0;
	int whiteScore = 0;
	const uint64_t fileMasks[8] = { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };

	//Checks for double and triple pawns.
	for (int x = 0; x < 8; x++)
	{
		int pawnsInRank = bitSum(board->whitePawnBitboard & fileMasks[x]);
		if (pawnsInRank > 1)
		{
			whiteScore -= 10 * (pawnsInRank - 1);
		}

		pawnsInRank = bitSum(board->blackPawnBitboard & fileMasks[x]);
		if (pawnsInRank > 1)
		{
			blackScore -= 10 * (pawnsInRank - 1);
		}
	}

	uint64_t currentPawn;
	int currentPos;

	const uint64_t whitePassedPawnMasks[8] = { 0xffffffffffffff00, 0xffffffffffff0000, 0xffffffffff000000,
		0xffffffff00000000, 0xffffff0000000000, 0xffff000000000000, 0xff00000000000000, 0 };
	const uint64_t neighbouringFileMasks[8] = { fileB, fileA + fileC, fileB + fileD, fileC + fileE, fileD + fileF, fileE + fileG, fileF + fileH, fileG };
	const uint64_t whiteBackwardsPawnMasks[8] = { 0xff, 0xffff, 0xffffff, 0xffffffff, 0xffffffffff, 
		0xffffffffffff, 0xffffffffffffff, 0xffffffffffffffff };
	const uint64_t blackPassedPawnMasks[8] = { 0, 0xff, 0xffff, 0xffffff, 0xffffffff, 0xffffffffff,
		0xffffffffffff, 0xffffffffffffff};
	const uint64_t blackBackwardsPawnMasks[8] = { 0xffffffffffffffff, 0xffffffffffffff00, 0xffffffffffff0000, 0xffffffffff000000,
		0xffffffff00000000,  0xffffff0000000000, 0xffff000000000000, 0xff00000000000000 };

	uint64_t whitePawnBitboard = board->whitePawnBitboard;
	while (whitePawnBitboard)
	{
		currentPawn = pop(whitePawnBitboard);
		currentPos = bitScanForward(currentPawn);

		//Passed pawns
		if (board->blackPawnBitboard & whitePassedPawnMasks[currentPos / 8] == 0)
		{
			whiteScore += 20 * (currentPos / 8);
		}

		//Isolated Pawns
		if (board->whitePawnBitboard & neighbouringFileMasks[currentPos % 8] == 0)
		{
			whiteScore -= 20;
		}
		//Backwards Pawns
		else if(board->whitePawnBitboard & neighbouringFileMasks[currentPos % 8] & whiteBackwardsPawnMasks[currentPos / 8] == 0)
		{
			whiteScore -= 8;
		}
	}

	uint64_t blackPawnBitboard = board->blackPawnBitboard;
	while (blackPawnBitboard)
	{
		currentPawn = pop(blackPawnBitboard);
		currentPos = bitScanForward(currentPawn);

		//Passed pawns
		if (board->whitePawnBitboard & blackPassedPawnMasks[currentPos / 8] == 0)
		{
			blackScore += 20 * (7 - (currentPos / 8));
		}

		//Isolated Pawns
		if (board->blackPawnBitboard & neighbouringFileMasks[currentPos % 8] == 0)
		{
			blackScore -= 20;
		}
		//Backwards Pawns
		else if (board->blackPawnBitboard & neighbouringFileMasks[currentPos % 8] & blackBackwardsPawnMasks[currentPos / 8] == 0)
		{
			blackScore -= 8;
		}
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

int calculateRookPositionScore(Board * board)
{
	int whiteScore = 0;
	int blackScore = 0;

	uint64_t currentRook;
	const uint64_t fileMasks[8] = { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };
	int currentPos;

	uint64_t whiteRookBitboard = board->whiteRookBitboard;
	while (whiteRookBitboard)
	{
		currentRook = pop(whiteRookBitboard);
		currentPos = bitScanForward(currentRook);

		//If the file of the rook contains no pawns.
		if (((board->whitePawnBitboard | board->blackPawnBitboard) & fileMasks[currentPos % 8]) == 0)
		{
			whiteScore += 15;
		}
		//If the file has black pawns but no white pawns.
		else if (board->whitePawnBitboard & fileMasks[currentPos % 8] == 0)
		{
			whiteScore += 10;
		}

		if ((currentPos / 8) == 6)
		{
			whiteScore += 20;
		}
	}

	uint64_t blackRookBitboard = board->blackRookBitboard;
	while (blackRookBitboard)
	{
		currentRook = pop(blackRookBitboard);
		currentPos = bitScanForward(currentRook);

		//If the file of the rook contains no pawns.
		if (((board->whitePawnBitboard | board->blackPawnBitboard) & fileMasks[currentPos % 8]) == 0)
		{
			blackScore += 15;
		}
		//If the file has white pawns but no black pawns.
		else if (board->blackPawnBitboard & fileMasks[currentPos % 8] == 0)
		{
			blackScore += 10;
		}

		if ((currentPos / 8) == 1)
		{
			blackScore += 20;
		}
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

int calculateMaterialScore(Board * board, bool lateGame)
{
	int whiteScore = 0;
	whiteScore += pieceSquareData::pawnSquare.calcScore(board->whitePawnBitboard, white);
	whiteScore += pieceSquareData::knightSquare.calcScore(board->whiteKnightBitboard, white);
	whiteScore += pieceSquareData::bishopSquare.calcScore(board->whiteBishopBitboard, white);

	whiteScore += bitSum(board->whiteRookBitboard) * 500;
	whiteScore += bitSum(board->whiteQueenBitboard) * 900;
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
	blackScore += bitSum(board->blackRookBitboard) * 500;
	blackScore += bitSum(board->blackQueenBitboard) * 900;
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

int calculateKingSafetyScore(Board * board)
{
	if (board->nextColour == white) return calculateKingSafetyScoreForColour(board, white) - calculateKingSafetyScoreForColour(board, black);
	if (board->nextColour == white) return calculateKingSafetyScoreForColour(board, black) - calculateKingSafetyScoreForColour(board, white);
}

int calculateKingSafetyScoreForColour(Board * board, colours colour)
{
	const uint64_t fileMasks[8] = { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };
	const uint64_t startingRankMask = rank2 ? colour == white : rank7;

	int score = 0;
	const uint64_t kingBitboard = board->findBitboard(colour, king);
	const uint64_t pawnBitboard = board->findBitboard(colour, pawn);
	const uint64_t enemyPawnBitboard = board->findBitboard(switchColour(colour), pawn);

	//If the king is in the three left hand files.
	if (kingBitboard & 506381209866536711)
	{
		//Iterate through the three files.
		for (int x = 0; x < 3; x++)
		{
			uint64_t fileMask = fileMasks[x];

			//File is not open
			if (pawnBitboard && fileMask)
			{
				//if the pawn has moved from its starting position, give a penalty
				if (!(pawnBitboard && fileMask && startingRankMask))
				{
					score -= 20;
				}
			}
			//Give a penalty for an open file.
			else
			{
				score -= 25;

				//Give a penalty for their being no enemy pawns on the file.
				if (!(enemyPawnBitboard && fileMask))
				{
					score -= 15;
				}
			}
		}
	}
	//If the king is in the three right hand files.
	else if (kingBitboard & 16204198715729174752)
	{
		//Iterate through the three files.
		for (int x = 5; x < 8; x++)
		{
			uint64_t fileMask = fileMasks[x];

			//File is not open
			if (pawnBitboard && fileMask)
			{
				//if the pawn has moved from its starting position, give a penalty
				if (!(pawnBitboard && fileMask && startingRankMask))
				{
					score -= 20;
				}
			}
			//Give a penalty for an open file.
			else
			{
				score -= 25;

				//Give a penalty for their being no enemy pawns on the file.
				if (!(enemyPawnBitboard && fileMask))
				{
					score -= 15;
				}
			}
		}
	}

	//King in the middle two files.
	else
	{
		const int kingFile = bitScanForward(kingBitboard) % 8;

		//Iterate through the three files on and adjacent to the king.
		for (int x = kingFile - 1; x < kingFile + 2; x++)
		{
			uint64_t fileMask = fileMasks[x];

			//File is not open
			if (pawnBitboard && fileMask)
			{
				//if the pawn has moved from its starting position, give a penalty
				if (!(pawnBitboard && fileMask && startingRankMask))
				{
					score -= 20;
				}
			}
			//Give a penalty for an open file.
			else
			{
				score -= 25;

				//Give a penalty for their being no enemy pawns on the file.
				if (!(enemyPawnBitboard && fileMask))
				{
					score -= 15;
				}
			}
		}
	}
	return score;
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
