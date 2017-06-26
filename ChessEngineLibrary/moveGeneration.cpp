#include "moveGeneration.h"

uint64_t knightMovesArray[64];
uint64_t kingMovesArray[64];

int searchForMoves(Board * board, std::array<Move,150>* moveList)
{
	int arraySize = 0;

	magicBitboards magicData;

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

	uint64_t captureMask = 0xFFFFFFFFFFFFFFFF;
	uint64_t pushMask = 0xFFFFFFFFFFFFFFFF;

	uint64_t kingDangerSquares = generateAttackSet(board, switchColour(board->nextColour), board->allPieces & ~board->findBitboard(board->nextColour, king));
	uint64_t pinnedPieces = getPinnedPieces(board);

	uint64_t kingAttackers;
	int numOfKingAttackers;

	//If in check
	if (kingDangerSquares & board->findBitboard(board->nextColour, king))
	{
		kingAttackers = getAttackers(board, board->nextColour, board->findBitboard(board->nextColour, king));
		numOfKingAttackers = bitSum(kingAttackers);
	}
	else
	{
		kingAttackers = 0;
		numOfKingAttackers = 0;
	}

	if (numOfKingAttackers == 1)
	{
		captureMask = kingAttackers;

		//If the attacker is a sliding piece
		if (kingAttackers & (board->findBitboard(switchColour(board->nextColour), rook) | board->findBitboard(switchColour(board->nextColour), queen) | board->findBitboard(switchColour(board->nextColour), bishop)))
		{
			pushMask = inBetween(bitScanForward(board->findBitboard(board->nextColour, king)), bitScanForward(kingAttackers));
		}
		else
		{
			pushMask = 0;
		}
	}
	if(numOfKingAttackers > 1) arraySize = generateKingMoves(board, moveList, friendlyPieces, enemyPieces, kingDangerSquares, arraySize);
	else 
	{
		arraySize = generatePawnMoves(board, moveList, pinnedPieces, pushMask, captureMask, arraySize);
		arraySize = generateKingMoves(board, moveList, friendlyPieces, enemyPieces, kingDangerSquares, arraySize);
		arraySize = generateKnightMoves(board, moveList, friendlyPieces, enemyPieces, pinnedPieces, pushMask, captureMask, arraySize);
		arraySize = generateRookMoves(board, moveList, friendlyPieces, enemyPieces, pinnedPieces, pushMask, captureMask, arraySize);
		arraySize = generateBishopMoves(board, moveList, friendlyPieces, enemyPieces, pinnedPieces, pushMask, captureMask, arraySize);
		arraySize = generateQueenMoves(board, moveList, friendlyPieces, enemyPieces, pinnedPieces, pushMask, captureMask, arraySize);
		arraySize = generateCastlingMoves(board, moveList, friendlyPieces, enemyPieces, kingDangerSquares, arraySize);
	}

	return arraySize;
}

int generatePawnMoves(Board* board, std::array<Move,150>* Movelist, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize)
{
	uint64_t pawnPos, pawnMoves, pawnAttacks, pawnDoubleMoves, legalMoves;
	if (board->nextColour == white)
	{
		uint64_t pawnBitboard = board->whitePawnBitboard;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);
			if (pawnPos & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, pawnPos);
			else legalMoves = ~0;

			pawnMoves = pawnPos << 8 & ~board->allPieces & legalMoves; //Move forward
			pawnDoubleMoves = ((pawnMoves & rank3) << 8) & ~board->allPieces & legalMoves & pushMask; //Move twice on first turn if first is clear
			pawnMoves &= pushMask;

			pawnAttacks = pawnPos << 7 & board->blackPieces & ~fileH | pawnPos << 9 & board->blackPieces & ~fileA;

			//Attack either enemy pieces or the en passent target square.
			if(board->enPassantSquare != -1)
			{
				uint64_t movedPieces = pawnPos | ((uint64_t)1 << board->enPassantSquare >> 8);
				if(!isPinnedEnPassant(board,movedPieces))
					pawnAttacks |= pawnPos << 7 & (uint64_t)1 << board->enPassantSquare & ~fileH | pawnPos << 9 & (uint64_t)1 << board->enPassantSquare & ~fileA;
			}


			pawnAttacks &= legalMoves & captureMask;

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnDoubleMoves)
			{
				uint64_t currentMove = pop(pawnDoubleMoves);
				(*Movelist)[arraySize] = Move(pawnPosIndex, bitScanForward(currentMove), pawnDoubleMove , pawn, board);
				arraySize++;
			}

			arraySize = addPawnMoves(pawnPosIndex, pawnMoves, pawnAttacks, board, Movelist, arraySize);
		}
	}
	else //Colour is black
	{
		uint64_t pawnBitboard = board->blackPawnBitboard;
		while (pawnBitboard)
		{
			pawnPos = pop(pawnBitboard);
			if (pawnPos & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, pawnPos);
			else legalMoves = ~0;

			pawnMoves = pawnPos >> 8 & ~board->allPieces & legalMoves; //Move forward
			pawnDoubleMoves = ((pawnMoves & rank6) >> 8) & ~board->allPieces & legalMoves & pushMask;  //Move twice on first turn if first is clear
			pawnMoves &= pushMask;

			pawnAttacks = pawnPos >> 9 & board->whitePieces &  ~fileH | pawnPos >> 7 & board->whitePieces & ~fileA;

			//Attack either enemy pieces or the en passent target square.
			if(board->enPassantSquare != -1)
			{
				uint64_t movedPieces = pawnPos | ((uint64_t)1 << board->enPassantSquare << 8);
				if(!isPinnedEnPassant(board,movedPieces))
					pawnAttacks |= (pawnPos >> 9) & (board->whitePieces | (uint64_t)1 << board->enPassantSquare) &  ~fileH | (pawnPos >> 7) & (board->whitePieces | (uint64_t)1 << board->enPassantSquare) & ~fileA;
			}
			else
			{
				pawnAttacks = (pawnPos >> 9) & (board->whitePieces) &  ~fileH | (pawnPos >> 7) & (board->whitePieces) & ~fileA;
			}
			pawnAttacks &= legalMoves & captureMask;

			int pawnPosIndex = bitScanForward(pawnPos);

			while (pawnDoubleMoves)
			{
				uint64_t currentMove = pop(pawnDoubleMoves);
				(*Movelist)[arraySize] = Move(pawnPosIndex, bitScanForward(currentMove), pawnDoubleMove, pawn, board);
				arraySize++;
			}

			arraySize = addPawnMoves(pawnPosIndex, pawnMoves, pawnAttacks, board, Movelist, arraySize);
		}
	}
	return arraySize;
}

int generateKingMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares, int arraySize)
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
	if (kingBitboard)
	{
		uint64_t moves = kingMovesArray[bitScanForward(kingBitboard)] & ~friendlyPieces;

		//Filters out moves that would move the king into check.
		moves &= ~kingDangerSquares;

		int kingPosIndex = bitScanForward(kingBitboard);
		while (moves)
		{
			uint64_t kingPos = pop(moves);
			arraySize = addMoves(kingPosIndex, bitScanForward(kingPos), king, Movelist, enemyPieces, board, arraySize);
		}
	}
	return arraySize;
}

int generateKnightMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize)
{
	uint64_t legalMoves;
	//Gets the knight bitboard , filtering out pieces that cannot move due to being pinned
	uint64_t knightBitboard = board->findBitboard(board->nextColour, knight) & ~pinnedPieces;
	
    while(knightBitboard)
    {
        uint64_t currentKnight = pop(knightBitboard);
		int knightPosIndex = bitScanForward(currentKnight);

		uint64_t moveBitboard = knightMovesArray[knightPosIndex] & ~friendlyPieces;
              
		//Filters out invalid moves while in check
		moveBitboard &= (pushMask | captureMask);

    	while (moveBitboard)
	    {
	       	uint64_t knightMove = pop(moveBitboard);
	    	arraySize = addMoves(knightPosIndex, bitScanForward(knightMove), knight, Movelist, enemyPieces, board, arraySize);
    	}
    }
	return arraySize;
}

int generateRookMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize)
{
	uint64_t rookBitboard, legalMoves;
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

		if (currentRook & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentRook);
		else legalMoves = ~0;

		uint64_t occupancy = magicBitboards::rookMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[currentPos];
		uint64_t moves = magicBitboards::magicMovesRook[currentPos][arrayIndex] & ~friendlyPieces & legalMoves & (pushMask | captureMask);

		while (moves)
		{
			uint64_t rookPos = pop(moves);
			arraySize = addMoves(currentPos, bitScanForward(rookPos), rook, Movelist, enemyPieces, board, arraySize);
		}
	}
	return arraySize;
}

int generateBishopMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize)
{
	uint64_t bishopBitboard, legalMoves;
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

		if (currentBishop & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentBishop);
		else legalMoves = ~0;

		uint64_t occupancy = magicBitboards::bishopMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[currentPos];
		uint64_t moves = magicBitboards::magicMovesBishop[currentPos][arrayIndex] & ~friendlyPieces & legalMoves & (pushMask | captureMask);

		while (moves)
		{
			uint64_t bishopPos = pop(moves);
			arraySize = addMoves(currentPos, bitScanForward(bishopPos), bishop, Movelist, enemyPieces, board, arraySize);
		}
	}
	return arraySize;
}

int generateQueenMoves(Board * board, std::array<Move,150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize)
{
	uint64_t queenBitboard, legalMoves;
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

		if (currentQueen & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentQueen);
		else legalMoves = ~0;

		//Moves bishop moves
		uint64_t occupancy = magicBitboards::bishopMask[currentPos] & board->allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[currentPos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[currentPos];
		uint64_t moves = magicBitboards::magicMovesBishop[currentPos][arrayIndex] & ~friendlyPieces & legalMoves & (pushMask | captureMask);

		//Rook moves
		occupancy = magicBitboards::rookMask[currentPos] & board->allPieces;
		magicResult = occupancy * magicBitboards::magicNumberRook[currentPos];
		arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[currentPos];
		moves |= magicBitboards::magicMovesRook[currentPos][arrayIndex] & ~friendlyPieces & legalMoves & (pushMask | captureMask);

		while (moves)
		{
			uint64_t queenPos = pop(moves);
			arraySize = addMoves(currentPos, bitScanForward(queenPos), queen, Movelist, enemyPieces, board,arraySize);
		}
	}
	return arraySize;
}

int generateCastlingMoves(Board * board, std::array<Move,150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares, int arraySize)
{
	if (board->nextColour == white)
	{
		if (board->canWhiteCastleKingSide && (board->allPieces & 96) == 0)//Kingside castling
		{
			if ((112 & kingDangerSquares) == 0)
			{
				(*Movelist)[arraySize] = Move(4, 6, kingSideCastling, king, board);
				arraySize++;
			}
		}
		if(board->canWhiteCastleQueenSide && (board->allPieces & 14) == 0)//Queenside castling
		{
			if ((28 & kingDangerSquares) == 0)
			{
				(*Movelist)[arraySize] = Move(4, 2, queenSideCastling, king, board);
				arraySize++;
			}
		}
	}
	else
	{
		if (board->canBlackCastleKingSide && (board->allPieces & 6917529027641081856) == 0)//Kingside castling
		{
			if ((8070450532247928832 & kingDangerSquares) == 0)
			{
				(*Movelist)[arraySize] = Move(60, 62, kingSideCastling, king, board);
				arraySize++;
			}
		}
		if (board->canBlackCastleQueenSide && (board->allPieces & 1008806316530991104) == 0)//Queenside castling
		{
			if ((2017612633061982208 & kingDangerSquares) == 0)
			{
				(*Movelist)[arraySize] = Move(60, 58, queenSideCastling, king, board);
				arraySize++;
			}
		}
	}
	return arraySize;
}

int addPawnMoves(int start, uint64_t quietMoves, uint64_t captureMoves, Board* board, std::array<Move,150>* Movelist, int arraySize)
{
	uint64_t currentMove;
	int currentPos;
	while (quietMoves)
	{
		currentMove = pop(quietMoves);
		currentPos = bitScanForward(currentMove);
		//Pawn promotion
		if ((board->nextColour == white && currentMove & rank8) || (board->nextColour == black && currentMove & rank1))
		{
			(*Movelist)[arraySize] = Move(start, currentPos, rookPromotion, pawn, board);
			(*Movelist)[arraySize + 1] = Move(start, currentPos, knightPromotion, pawn, board);
			(*Movelist)[arraySize + 2] = Move(start, currentPos, queenPromotion, pawn, board);
			(*Movelist)[arraySize + 3] = Move(start, currentPos, bishopPromotion, pawn, board);
			arraySize += 4;
		}
		else
		{
			(*Movelist)[arraySize] = Move(start, currentPos, quietMove, pawn, board);
			arraySize++;
		}
	}
	while (captureMoves)
	{
		currentMove = pop(captureMoves);
		currentPos = bitScanForward(currentMove);
		//Pawn promotion
		if ((board->nextColour == white && currentMove & rank8) || (board->nextColour == black && currentMove & rank1))
		{
			(*Movelist)[arraySize] = Move(start, currentPos, rookPromotion, pawn, board);
			(*Movelist)[arraySize + 1] = Move(start, currentPos, knightPromotion, pawn, board);
			(*Movelist)[arraySize + 2] = Move(start, currentPos, queenPromotion, pawn, board);
			(*Movelist)[arraySize + 3] = Move(start, currentPos, bishopPromotion, pawn, board);
			arraySize += 4;
		}
		else
		{
			(*Movelist)[arraySize] = Move(start, currentPos, capture, pawn, board);
			arraySize++;
		}
	}
	return arraySize;
}

int addMoves(int start, int end, pieceType piece, std::array<Move,150>* Movelist, uint64_t enemyPieces, Board* board, int arraySize)
{
	if ((((uint64_t)1 << end) & enemyPieces )!= 0) //If the move is a capture
	{
		(*Movelist)[arraySize] = Move(start, end, capture, piece, board);
		arraySize++;
	}
	else
	{
		(*Movelist)[arraySize] = Move(start, end, quietMove, piece, board);
		arraySize++;
	}
	return arraySize;
}

uint64_t generateAttackSet(Board * board, colours colour, uint64_t allPieces)
{
	uint64_t attackSet = 0;
	uint64_t currentPos;

	uint64_t pawnBitboard = board->findBitboard(colour, pawn);
	if (colour == white)
	{
		while (pawnBitboard)
		{
			currentPos = pop(pawnBitboard);
			attackSet |= currentPos << 7 & ~fileH | currentPos << 9 & ~fileA;
		}
	}
	else
	{
		while (pawnBitboard)
		{
			currentPos = pop(pawnBitboard);
			attackSet |= (currentPos >> 9) & ~fileH | (currentPos >> 7) & ~fileA;
		}
	}

	uint64_t knightBitboard = board->findBitboard(colour, knight);
	while (knightBitboard)
	{
		currentPos = pop(knightBitboard);
		attackSet |= knightMovesArray[bitScanForward(currentPos)];
	}

	uint64_t kingBitboard = board->findBitboard(colour, king);
	while (kingBitboard)
	{
		currentPos = pop(kingBitboard);
		attackSet |= kingMovesArray[bitScanForward(currentPos)];
	}

	uint64_t rookBitboard = board->findBitboard(colour, rook) | board->findBitboard(colour, queen);
	while (rookBitboard)
	{
		currentPos = pop(rookBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::rookMask[piecePos] & allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[piecePos];
		attackSet |= magicBitboards::magicMovesRook[piecePos][arrayIndex];
	}

	uint64_t bishopBitboard = board->findBitboard(colour, bishop) | board->findBitboard(colour, queen);
	while (bishopBitboard)
	{
		currentPos = pop(bishopBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::bishopMask[piecePos] & allPieces;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[piecePos];
		attackSet |= magicBitboards::magicMovesBishop[piecePos][arrayIndex];
	}

	return attackSet;
}

uint64_t getPinnedPieces(Board * board)
{
	uint64_t pinnedPieces = 0;

	uint64_t kingBitBoard = board->findBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	uint64_t enemyPieces, friendlyPieces;
	if (board->nextColour == white)
	{
		enemyPieces = board->blackPieces;
		friendlyPieces = board->whitePieces;
	}
	else
	{
		enemyPieces = board->whitePieces;
		friendlyPieces = board->blackPieces;
	}

	uint64_t occupancy = magicBitboards::rookMask[kingPos] & enemyPieces;
	uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[kingPos];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[kingPos];
	uint64_t kingRaysRook = magicBitboards::magicMovesRook[kingPos][arrayIndex] & (board->findBitboard(switchColour(board->nextColour), rook) | board->findBitboard(switchColour(board->nextColour), queen));
	while (kingRaysRook)
	{
		uint64_t pinner = pop(kingRaysRook);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & friendlyPieces;
		if (bitSum(pinnedPiece) == 1) pinnedPieces |= pinnedPiece;
	}


	occupancy = magicBitboards::bishopMask[kingPos] & enemyPieces;
	magicResult = occupancy * magicBitboards::magicNumberBishop[kingPos];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[kingPos];
	uint64_t kingRaysBishop = magicBitboards::magicMovesBishop[kingPos][arrayIndex] & (board->findBitboard(switchColour(board->nextColour), bishop) | board->findBitboard(switchColour(board->nextColour), queen));;
	while (kingRaysBishop)
	{
		uint64_t pinner = pop(kingRaysBishop);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & friendlyPieces;
		if (bitSum(pinnedPiece) == 1) pinnedPieces |= pinnedPiece;
	}

	return pinnedPieces;
}

//Calculates the moves a pinned piece could move and stay out of check.
uint64_t generateLegalFilterForPinnedPiece(Board* board, uint64_t pinnedPiece)
{
	uint64_t allPiecesWithoutPiece = board->allPieces & ~pinnedPiece;
	uint64_t kingBitBoard = board->findBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	uint64_t currentPos, currentRay, rookRays, bishopRays;
	uint64_t rookBitboard = board->findBitboard(switchColour(board->nextColour), rook) | board->findBitboard(switchColour(board->nextColour), queen);
	while (rookBitboard)
	{
		currentPos = pop(rookBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::rookMask[piecePos] & allPiecesWithoutPiece;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[piecePos];
		rookRays = magicBitboards::magicMovesRook[piecePos][arrayIndex] & kingBitBoard;
		if (inBetween(kingPos, piecePos) & pinnedPiece)
		{
			return inBetween(kingPos, piecePos) | currentPos;
		}
		
	}

	uint64_t bishopBitboard = board->findBitboard(switchColour(board->nextColour), bishop) | board->findBitboard(switchColour(board->nextColour), queen);
	while (bishopBitboard)
	{
		currentPos = pop(bishopBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::bishopMask[piecePos] & allPiecesWithoutPiece;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[piecePos];
		bishopRays = magicBitboards::magicMovesBishop[piecePos][arrayIndex] & kingBitBoard;
		if (inBetween(kingPos, piecePos) & pinnedPiece)
		{
			return inBetween(kingPos, piecePos) | currentPos;
		}
		
	}

	return 0;
}

uint64_t getAttackers(Board * board, colours colour, uint64_t targetBitboard)
{
	colours opponentColour = switchColour(colour);
	int targetPos = bitScanForward(targetBitboard);
	uint64_t attackers = 0;

	if (colour == white)
	{
		attackers |= board->findBitboard(opponentColour, pawn) & (((targetBitboard << 7) & ~fileH) | ((targetBitboard << 9) & ~fileA));
	}
	else
	{
		attackers |= board->findBitboard(opponentColour, pawn) & (((targetBitboard >> 9) & ~fileH) | ((targetBitboard >> 7) & ~fileA));
	}

	//KnightMoves
	uint64_t knightMoves = knightMovesArray[targetPos];
	attackers |= knightMoves & board->findBitboard(opponentColour, knight);

	uint64_t moves = kingMovesArray[targetPos];
	attackers |= moves & board->findBitboard(opponentColour, king);
	
	//Rook and half of queen moves
	uint64_t occupancy = magicBitboards::rookMask[targetPos] & board->allPieces;
	uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[targetPos];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[targetPos];
	uint64_t magicMoves = magicBitboards::magicMovesRook[targetPos][arrayIndex];

	attackers |= magicMoves & (board->findBitboard(opponentColour, rook) | (board->findBitboard(opponentColour, queen)));
	
	//Bishop and half of queen moves
	occupancy = magicBitboards::bishopMask[targetPos] & board->allPieces;
	magicResult = occupancy * magicBitboards::magicNumberBishop[targetPos];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[targetPos];
	magicMoves = magicBitboards::magicMovesBishop[targetPos][arrayIndex];

	attackers |= magicMoves & (board->findBitboard(opponentColour, bishop) | (board->findBitboard(opponentColour, queen)));

	return attackers;
}

void setupMoveGen()
{
	uint64_t currentPos, knightMoves, kingMoves;
	for (int x = 0; x < 64; x++)
	{
		currentPos = (uint64_t)1 << x;

		knightMoves = 0;
		knightMoves |= currentPos << 15 & ~fileH;
		knightMoves |= currentPos << 17 & ~fileA;
		knightMoves |= currentPos << 06 & ~fileH & ~fileG;
		knightMoves |= currentPos << 10 & ~fileA & ~fileB;
		knightMoves |= currentPos >> 10 & ~fileH & ~fileG;
		knightMoves |= currentPos >> 06 & ~fileA & ~fileB;
		knightMoves |= currentPos >> 17 & ~fileH;
		knightMoves |= currentPos >> 15 & ~fileA;
		knightMovesArray[x] = knightMoves;

		kingMoves = 0;
		kingMoves |= currentPos << 8;
		kingMoves |= currentPos << 9 & ~fileA;
		kingMoves |= currentPos << 1 & ~fileA;
		kingMoves |= currentPos >> 7 & ~fileA;
		kingMoves |= currentPos >> 8;
		kingMoves |= currentPos >> 9 & ~fileH;
		kingMoves |= currentPos >> 1 & ~fileH;
		kingMoves |= currentPos << 7 & ~fileH;
		kingMovesArray[x] = kingMoves;
	}
}

bool isPinnedEnPassant(Board* board, uint64_t pieces)
{
	uint64_t kingBitBoard = board->findBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	uint64_t magicResult = 0 * magicBitboards::magicNumberRook[kingPos];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[kingPos];
	uint64_t kingRaysRook = magicBitboards::magicMovesRook[kingPos][arrayIndex];
	kingRaysRook &= (board->findBitboard(switchColour(board->nextColour), rook) | board->findBitboard(switchColour(board->nextColour), queen));
	while (kingRaysRook)
	{
		uint64_t pinner = pop(kingRaysRook);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & board->allPieces;
		if (pinnedPiece ==  pieces) return true;
	}
	/*
	magicResult = 0 * magicBitboards::magicNumberBishop[kingPos];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[kingPos];
	uint64_t kingRaysBishop = magicBitboards::magicMovesBishop[kingPos][arrayIndex];
	kingRaysBishop &= (board->findBitboard(switchColour(board->nextColour), bishop) | board->findBitboard(switchColour(board->nextColour), queen));
	while (kingRaysBishop)
	{
		uint64_t pinner = pop(kingRaysBishop);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & board->allPieces;
		if (pinnedPiece & pieces > 0 && bitSum(pinnedPiece) == bitSum(pinnedPiece & pieces)) return true;
	}
	*/
	return false;
}
