#include "moveGeneration.h"

std::vector<Move> searchForMoves(Board * board, colours aiColour)
{
	std::vector<Move> moveList;
	magicBitboards magicData;

	uint64_t friendlyPieces, enemyPieces;
	if (aiColour == white)
	{
		friendlyPieces = board->whitePieces;
		enemyPieces = board->blackPieces;
	}
	else
	{
		friendlyPieces = board->blackPieces;
		enemyPieces = board->whitePieces;
	}

	generatePawnMoves(board, aiColour, moveList);
	generateKingMoves(board, aiColour, moveList,  friendlyPieces,  enemyPieces);
	generateKnightMoves(board, aiColour, moveList,  friendlyPieces,  enemyPieces);
	generateRookMoves(board, aiColour, moveList,  friendlyPieces,  enemyPieces);
	generateBishopMoves(board, aiColour, moveList,  friendlyPieces,  enemyPieces);
	generateQueenMoves(board, aiColour, moveList,  friendlyPieces,  enemyPieces);

	return moveList;
}

void generatePawnMoves(Board* board, colours aiColour, std::vector<Move>& Movelist)
{
	if (aiColour == white)
	{
		uint64_t pawnBitboard = board->whitePawnBitboard;
		uint64_t pawnPos, pawnMoves, pawnAttacks;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos << 8 & ~board->allPieces; //Move forward
			pawnMoves |= ((pawnMoves & rank3) << 8) & ~board->allPieces; //Move twice on first turn if first is clear
			pawnAttacks = board->whitePawnBitboard << 7 & board->blackPieces & fileH | board->whitePawnBitboard << 9 & board->blackPieces & ~fileA; //Attack

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnMoves)
			{
				uint64_t pawnMove = pop(pawnMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnMove), quietMove, pawn));
			}
			while (pawnAttacks)
			{
				uint64_t pawnAttack = pop(pawnAttacks);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnAttack), capture, pawn));
			}

			if ((pawnPos & rank7) > 0)//Pawn Promotion
			{
				int end = pawnPosIndex + 8;
				Movelist.push_back(Move(pawnPosIndex, end, rookPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, knightPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, queenPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, bishopPromotion, pawn));
			}
		}
	}
	else //Colour is black
	{
		uint64_t pawnBitboard = board->blackPawnBitboard;
		uint64_t pawnPos, pawnMoves, pawnAttacks;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos >> 8 & ~board->allPieces; //Move forward
			pawnMoves |= ((pawnMoves & rank3) << 8) & ~board->allPieces;  //Move twice on first turn if first is clear
			pawnAttacks = (pawnPos >> 9) & board->whitePieces & ~fileH | (pawnPos >> 7) & board->whitePieces & ~fileA; //Attack

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnMoves)
			{
				uint64_t pawnMove = pop(pawnMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnMove), quietMove, pawn));
			}
			while (pawnAttacks)
			{
				uint64_t pawnAttack = pop(pawnAttacks);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(pawnAttack), capture, pawn));
			}

			if ((pawnPos & rank2) > 0)//Pawn Promotion
			{
				int end = pawnPosIndex - 8;
				Movelist.push_back(Move(pawnPosIndex, end, rookPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, knightPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, queenPromotion, pawn));
				Movelist.push_back(Move(pawnPosIndex, end, bishopPromotion, pawn));
			}
		}
	}
}

void generateKingMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t kingBitboard;
	if (aiColour == white)
	{
		kingBitboard = board->whiteKingBitboard;
	}
	else
	{
		kingBitboard = board->blackKingBitboard;
	}
	uint64_t moves = kingBitboard << 8 & ~friendlyPieces;
	moves |= kingBitboard << 9 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard << 1 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard >> 7 & ~friendlyPieces & ~fileA;
	moves |= kingBitboard >> 8 & ~friendlyPieces;
	moves |= kingBitboard >> 9 & ~friendlyPieces & ~fileH;
	moves |= kingBitboard >> 1 & ~friendlyPieces & ~fileH;
	moves |= kingBitboard << 7 & ~friendlyPieces & ~fileH;

	int kingPosIndex = bitScanForward(kingBitboard);
	while (moves)
	{
		uint64_t kingPos = pop(moves);
		addMoves(kingPosIndex, bitScanForward(kingPos), king, Movelist, enemyPieces);
	}
}

void generateKnightMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t knightBitboard;
	if (aiColour == white)
	{
		knightBitboard = board->whiteKnightBitboard;
	}
	else
	{
		knightBitboard = board->blackKnightBitboard;
	}
	
    while(knightBitboard)
    {
        uint64_t currentKnight = pop(knightBitboard);
		uint64_t moveBitboard = 0;
        
        moveBitboard |= (currentKnight << 15) & ~friendlyPieces & ~fileH; //2 up 1 left
        moveBitboard |= (currentKnight << 17) & ~friendlyPieces & ~fileA; //2 up 1 Right
        moveBitboard |= (currentKnight << 06) & ~friendlyPieces & ~fileH & ~fileG;//1 up 2 left
        moveBitboard |= (currentKnight << 10) & ~friendlyPieces & ~fileA & ~fileB;//1 up 2 right
        moveBitboard |= (currentKnight >> 10) & ~friendlyPieces & ~fileH & ~fileG;//1 down 2 left
        moveBitboard |= (currentKnight >> 06) & ~friendlyPieces & ~fileA & ~fileB;//1 down 2 right
        moveBitboard |= (currentKnight >> 17) & ~friendlyPieces & ~fileH;//2 down 1 left
        moveBitboard |= (currentKnight >> 15) & ~friendlyPieces & ~fileA;//2 down 1 right
       
	    int knightPosIndex = bitScanForward(currentKnight);
    	while (moveBitboard)
	    {
	       	uint64_t knightPos = pop(moveBitboard);
	    	addMoves(knightPosIndex, bitScanForward(knightPos), knight, Movelist, enemyPieces);
    	}
    }


}

void generateRookMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t rookBitboard;
	if (aiColour == white)
	{
		rookBitboard = board->whiteRookBitboard;
	}
	else
	{
		rookBitboard = board->blackRookBitboard;
	}
	while (rookBitboard)
	{
		uint64_t currentRook = pop(rookBitboard);
		int currentPos = bitScanForward(currentRook);

		uint64_t occupancy = magicBitboards::rookMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[currentPos];
		uint64_t moves = magicBitboards::magicMovesRook[currentPos][arrayIndex] & ~friendlyPieces;

		while (moves)
		{
			uint64_t rookPos = pop(moves);
			addMoves(currentPos, bitScanForward(rookPos), rook, Movelist, enemyPieces);
		}
	}
}

void generateBishopMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t bishopBitboard;
	if (aiColour == white)
	{
		bishopBitboard = board->whiteBishopBitboard;
	}
	else
	{
		bishopBitboard = board->blackBishopBitboard;
	}
	while (bishopBitboard)
	{
		uint64_t currentBishop = pop(bishopBitboard);
		int currentPos = bitScanForward(currentBishop);

		uint64_t occupancy = magicBitboards::bishopMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[currentPos];
		uint64_t moves = magicBitboards::magicMovesBishop[currentPos][arrayIndex] & ~friendlyPieces;

		while (moves)
		{
			uint64_t bishopPos = pop(moves);
			addMoves(currentPos, bitScanForward(bishopPos), bishop, Movelist, enemyPieces);
		}
	}
}

void generateQueenMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t queenBitboard;
	if (aiColour == white)
	{
		queenBitboard = board->whiteQueenBitboard;
	}
	else
	{
		queenBitboard = board->blackQueenBitboard;
	}
	while (queenBitboard)
	{
		uint64_t currentQueen = pop(queenBitboard);
		int currentPos = bitScanForward(currentQueen);

		//Moves bishop moves
		uint64_t occupancy = magicBitboards::bishopMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[currentPos];
		uint64_t moves = magicBitboards::magicMovesBishop[currentPos][arrayIndex] & ~friendlyPieces;

		//Rook moves
		occupancy = magicBitboards::rookMask[currentPos] & board->allPieces;
		magicResult = occupancy * magicBitboards::magicNumberRook[currentPos];
		arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[currentPos];
		moves |= magicBitboards::magicMovesRook[currentPos][arrayIndex] & ~friendlyPieces;

		while (moves)
		{
			uint64_t queenPos = pop(moves);
			addMoves(currentPos, bitScanForward(queenPos), queen, Movelist, enemyPieces);
		}
	}
}

void addMoves(int start, int end, pieceType piece, std::vector<Move>& Movelist, uint64_t enemyPieces)
{
	if ((((uint64_t)1 << end) & enemyPieces )!= 0) //If the move is a capture
	{
		Movelist.push_back(Move(start, end, capture, piece));
	}
	else
	{
		Movelist.push_back(Move(start, end, quietMove, piece));
	}
}
