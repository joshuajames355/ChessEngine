#include "moveGeneration.h"

std::vector<Move> searchForMoves(Board * board)
{
	std::vector<Move> moveList;
	magicBitboards magicData;
	moveList.reserve(150);

	uint64_t friendlyPieces, enemyPieces;
	if (board->nextColour == white)
	{
		friendlyPieces = board->whitePieces;
		enemyPieces = board->blackPieces;
	}
	else
	{
		friendlyPieces = board->blackPieces;
		enemyPieces = board->whitePieces;
	}

	generatePawnMoves(board, moveList);
	generateKingMoves(board, moveList,  friendlyPieces,  enemyPieces);
	generateKnightMoves(board, moveList,  friendlyPieces,  enemyPieces);
	generateRookMoves(board, moveList,  friendlyPieces,  enemyPieces);
	generateBishopMoves(board, moveList,  friendlyPieces,  enemyPieces);
	generateQueenMoves(board, moveList,  friendlyPieces,  enemyPieces);

	moveList.shrink_to_fit();

	return moveList;
}

void generatePawnMoves(Board* board, std::vector<Move>& Movelist)
{
	if (board->nextColour == white)
	{
		uint64_t pawnBitboard = board->whitePawnBitboard;
		uint64_t pawnPos, pawnMoves, pawnAttacks, pawnDoubleMoves;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos << 8 & ~board->allPieces; //Move forward
			pawnDoubleMoves = ((pawnMoves & rank3) << 8) & ~board->allPieces; //Move twice on first turn if first is clear

			//Attack either enemy pieces or the en passent target square.
			pawnAttacks = board->whitePawnBitboard << 7 & (board->blackPieces | (uint64_t)1 << board->enPassantSquare) & ~fileH | board->whitePawnBitboard << 9 & (board->blackPieces | (uint64_t)1 << board->enPassantSquare) & ~fileA; 

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
			while (pawnDoubleMoves)
			{
				uint64_t currentMove = pop(pawnDoubleMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(currentMove), pawnDoubleMove , pawn));
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
		uint64_t pawnPos, pawnMoves, pawnAttacks, pawnDoubleMoves;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);

			pawnMoves = pawnPos >> 8 & ~board->allPieces; //Move forward
			pawnDoubleMoves = ((pawnMoves & rank3) << 8) & ~board->allPieces;  //Move twice on first turn if first is clear

			//Attack either enemy pieces or the en passent target square.
			pawnAttacks = (pawnPos >> 9) & (board->whitePieces | (uint64_t)1 << board->enPassantSquare) &  ~fileH | (pawnPos >> 7) & (board->whitePieces | (uint64_t)1 << board->enPassantSquare) & ~fileA;

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
			while (pawnDoubleMoves)
			{
				uint64_t currentMove = pop(pawnDoubleMoves);
				Movelist.push_back(Move(pawnPosIndex, bitScanForward(currentMove), pawnDoubleMove, pawn));
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

void generateKingMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t kingBitboard;
	if (board->nextColour == white)
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

void generateKnightMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t knightBitboard;
	if (board->nextColour == white)
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

void generateRookMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t rookBitboard;
	if (board->nextColour == white)
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

void generateBishopMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t bishopBitboard;
	if (board->nextColour == white)
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

void generateQueenMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces)
{
	uint64_t queenBitboard;
	if (board->nextColour == white)
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
