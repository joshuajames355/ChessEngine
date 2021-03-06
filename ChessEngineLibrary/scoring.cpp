#include "scoring.h"

//Pawn structure hash table
PawnStructureTableEntry pawnHashTable[2048];

int calculateScoreDiff(Board* board)
{
	int score = calculatePawnStructureScore(board);
	score += calculateMaterialScore(board);
	score += calculateRookPositionScore(board);
	score += calculateKingSafetyScore(board);

	return score;
}

int calculatePawnStructureScore(Board* board)
{
	//If this pawn structure has already been calculated.
	if (pawnHashTable[board->pawnScoreZorbistKey % 2048].zorbistKey == board->pawnScoreZorbistKey && board->pawnScoreZorbistKey != 0)
	{
		//std::cout << "\n\nmatch found\n\n";
		//board->printBoard();
		//std::cout << "\n\n";
		//pawnHashTable[board->pawnScoreZorbistKey % 2048].prevBoard.printBoard();
		//std::cout << "\n\n" << pawnHashTable[board->pawnScoreZorbistKey % 2048].score << "\n\n";

		if (board->nextColour == white) return pawnHashTable[board->pawnScoreZorbistKey % 2048].score;
		else return -pawnHashTable[board->pawnScoreZorbistKey % 2048].score;
	}

	int blackScore = 0;
	int whiteScore = 0;
	const uint64_t fileMasks[8] = { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };

	//Checks for double and triple pawns.
	for (int x = 0; x < 8; x++)
	{
		int pawnsInRank = bitSum(board->getPieceBitboard(white,pawn) & fileMasks[x]);
		if (pawnsInRank > 1)
		{
			whiteScore -= 10 * (pawnsInRank - 1);
		}

		pawnsInRank = bitSum(board->getPieceBitboard(black,pawn) & fileMasks[x]);
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

	uint64_t whitePawnBitboard = board->getPieceBitboard(white,pawn);
	while (whitePawnBitboard)
	{
		currentPawn = pop(whitePawnBitboard);
		currentPos = bitScanForward(currentPawn);

		//Passed pawns
		if ((board->getPieceBitboard(black,pawn) & whitePassedPawnMasks[currentPos / 8] & (neighbouringFileMasks[currentPos % 8] | fileMasks[currentPos % 8])) == 0)
		{
			whiteScore += 20 * (currentPos / 8);
		}

		//Isolated Pawns
		if ((board->getPieceBitboard(white,pawn) & neighbouringFileMasks[currentPos % 8]) == 0)
		{
			whiteScore -= 20;
		}
		//Backwards Pawns
		else if((board->getPieceBitboard(white,pawn) & neighbouringFileMasks[currentPos % 8] & whiteBackwardsPawnMasks[currentPos / 8]) == 0)
		{
			whiteScore -= 8;
		}
	}

	uint64_t blackPawnBitboard = board->getPieceBitboard(black,pawn);
	while (blackPawnBitboard)
	{
		currentPawn = pop(blackPawnBitboard);
		currentPos = bitScanForward(currentPawn);

		//Passed pawns

		if ((board->getPieceBitboard(white,pawn) & blackPassedPawnMasks[currentPos / 8] & (neighbouringFileMasks[currentPos % 8] | fileMasks[currentPos % 8])) == 0)
		{
			blackScore += 20 * (7 - (currentPos / 8));
		}

		//Isolated Pawns
		if ((board->getPieceBitboard(black,pawn) & neighbouringFileMasks[currentPos % 8]) == 0)
		{
			blackScore -= 20;
		}
		//Backwards Pawns
		else if ((board->getPieceBitboard(black,pawn) & neighbouringFileMasks[currentPos % 8] & blackBackwardsPawnMasks[currentPos / 8]) == 0)
		{
			blackScore -= 8;
		}
	}

	//Caches results into hash table.
	pawnHashTable[board->pawnScoreZorbistKey % 2048].zorbistKey = board->pawnScoreZorbistKey;
	pawnHashTable[board->pawnScoreZorbistKey % 2048].score = whiteScore - blackScore;
	//pawnHashTable[board->pawnScoreZorbistKey % 2048].prevBoard = *board;

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

	uint64_t whiteRookBitboard = board->getPieceBitboard(white, rook);
	while (whiteRookBitboard)
	{
		currentRook = pop(whiteRookBitboard);
		currentPos = bitScanForward(currentRook);

		//If the file of the rook contains no pawns.
		if (((board->getPieceBitboard(white,pawn) | board->getPieceBitboard(black,pawn)) & fileMasks[currentPos % 8]) == 0)
		{
			whiteScore += 15;
		}
		//If the file has black pawns but no white pawns.
		else if ((board->getPieceBitboard(white,pawn) & fileMasks[currentPos % 8]) == 0)
		{
			whiteScore += 10;
		}

		if ((currentPos / 8) == 6)
		{
			whiteScore += 20;
		}
	}

	uint64_t blackRookBitboard = board->getPieceBitboard(black, rook);
	while (blackRookBitboard)
	{
		currentRook = pop(blackRookBitboard);
		currentPos = bitScanForward(currentRook);

		//If the file of the rook contains no pawns.
		if (((board->getPieceBitboard(white,pawn) | board->getPieceBitboard(black,pawn)) & fileMasks[currentPos % 8]) == 0)
		{
			blackScore += 15;
		}
		//If the file has white pawns but no black pawns.
		else if ((board->getPieceBitboard(black,pawn) & fileMasks[currentPos % 8]) == 0)
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

int calculateMaterialScore(Board * board)
{
	int whiteScore = board->getPositionalScore(white) + board->getMaterialScore(white);
	//If late game
	if (board->getOnlyMaterialScore(black) <= 1200)
	{
		whiteScore += board->getLateGameKingPositionalScore(white);
	}

	int blackScore = 0;
	//If late game
	if (board->getOnlyMaterialScore(white) <= 1200)
	{
		blackScore = board->getLateGameKingPositionalScore(black);
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
	int score = 0;
	//If midgame for white
	if (board->getOnlyMaterialScore(black) > 1200)
	{
		const float whiteMultiplier = ((float)board->getOnlyMaterialScore(black) / 3100.0);
		score += whiteMultiplier * (float)(calculateKingSafetyScoreForColour(board, white) + board->getMidGameKingPositionalScore(white));
	}
	//If midgame for black
	if (board->getOnlyMaterialScore(white) > 1200)
	{
		const float blackMultiplier = ((float)board->getOnlyMaterialScore(white) / 3100.0);
		score -= blackMultiplier * (float)(calculateKingSafetyScoreForColour(board, black) + board->getMidGameKingPositionalScore(black));
	}

	if (board->nextColour == white) return score;
	else return -score;
}

int calculateKingSafetyScoreForColour(Board * board, colours colour)
{
	const uint64_t fileMasks[8] = { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };
	const uint64_t startingRankMask = colour == white ? rank2 : rank7;
	const uint64_t movedOnceMask = colour == white ? rank3 : rank6;
	const uint64_t movedTwiceMask = colour == white ? rank4 : rank5;

	int score = 0;
	const uint64_t kingBitboard = board->getPieceBitboard(colour, king);
	const uint64_t pawnBitboard = board->getPieceBitboard(colour, pawn);
	const uint64_t enemyPawnBitboard = board->getPieceBitboard(switchColour(colour), pawn);

	//If the king is in the three left hand files.
	if (kingBitboard & 506381209866536711)
	{
		//Iterate through the three files.
		for (int x = 0; x < 3; x++)
		{
			uint64_t fileMask = fileMasks[x];
			
			//Half the scores for files 2 and 5
			const float scoreMultiplier = x != 2 ? 1 : 0.5;
			int fileScore = 0;

			//File is not open
			if (pawnBitboard & fileMask)
			{
				//If the pawn has moved
				if ((pawnBitboard & fileMask & startingRankMask) == 0)
				{
					//if the pawn has moved more than once, give a penalty
					if ((pawnBitboard & fileMask & movedOnceMask)  == 0)
					{
						fileScore -= 20;
					}
					else
					{
						fileScore -= 10;
					}
				}			
			}
			//Give a penalty for an open file.
			else
			{
				fileScore -= 25;

			}

			//Give a penalty for their being no enemy pawns on the file. Semi-open
			if ((enemyPawnBitboard & fileMask) == 0)
			{
				fileScore -= 15;
			}
			else
			{
				//If the pawn is in front of your starting rank
				if (enemyPawnBitboard & movedOnceMask & fileMask)
				{
					fileScore -= 10;
				}
				//If the pawn is on your front rank.
				else if (enemyPawnBitboard & movedTwiceMask & fileMask)
				{
					fileScore -= 5;
				}
			}

			score += fileScore * scoreMultiplier;
		}

	}
	//If the king is in the three right hand files.
	else if (kingBitboard & 16204198715729174752)
	{
		//Iterate through the three files.
		for (int x = 5; x < 8; x++)
		{
			uint64_t fileMask = fileMasks[x];

			//Half the scores for files 2 and 5
			const float scoreMultiplier = x != 5 ? 1 : 0.5;
			int fileScore = 0;

			//File is not open
			if (pawnBitboard & fileMask)
			{
				//If the pawn has moved
				if ((pawnBitboard & fileMask & startingRankMask) == 0)
				{
					//if the pawn has moved more than once, give a penalty
					if ((pawnBitboard & fileMask & movedOnceMask) == 0)
					{
						fileScore -= 20;
					}
					else
					{
						fileScore -= 10;
					}
				}
			}
			//Give a penalty for an open file.
			else
			{
				fileScore -= 25;

			}

			//Give a penalty for their being no enemy pawns on the file. Semi-open
			if ((enemyPawnBitboard & fileMask) == 0)
			{
				fileScore -= 15;
			}
			else
			{
				//If the pawn is in front of your starting rank
				if (enemyPawnBitboard & movedOnceMask & fileMask)
				{
					fileScore -= 10;
				}
				//If the pawn is on your front rank.
				else if (enemyPawnBitboard & movedTwiceMask & fileMask)
				{
					fileScore -= 5;
				}
			}
			score += fileScore * scoreMultiplier;
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

			//If the file is fully open
			if ((fileMask & (board->getPieceBitboard(white,pawn) | board->getPieceBitboard(black,pawn))) == 0)
			{
				score -= 10;
			}
		}
	}
	return score;
}