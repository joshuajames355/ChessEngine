#include "moveGeneration.h"

uint64_t knightMovesArray[64] = {0};
uint64_t kingMovesArray[64] = { 0 };
uint64_t pawnWhiteAttacksArray[64] = { 0 };
uint64_t pawnBlackAttacksArray[64] = { 0 };

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

	uint64_t kingDangerSquares = board->getKingDangerSquares();
	uint64_t pinnedPieces = getPinnedPieces(board);

	uint64_t kingAttackers;
	int numOfKingAttackers;

	//If in check
	if (kingDangerSquares & board->getPieceBitboard(board->nextColour, king))
	{
		kingAttackers = getAttackers(board, board->nextColour, board->getPieceBitboard(board->nextColour, king));
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
		if (kingAttackers & (board->getPieceBitboard(switchColour(board->nextColour), rook) | board->getPieceBitboard(switchColour(board->nextColour), queen) | board->getPieceBitboard(switchColour(board->nextColour), bishop)))
		{
			pushMask = inBetween(bitScanForward(board->getPieceBitboard(board->nextColour, king)), bitScanForward(kingAttackers));
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
	bool isPinned;

	//Calculates constants used later
	const int forwards = (board->nextColour == white) ? 8 : -8;
	const uint64_t rank3BB = (board->nextColour == white) ? rank3 : rank6;
	const uint64_t emptySquares = ~board->allPieces;
	//const uint64_t emptySquares = ~board->allPieces & pushMask;
	const int leftAttack = (board->nextColour == white) ? 7 : -9;
	const int rightAttack = (board->nextColour == white) ? 9 : -7;
	const uint64_t enemyPieces = ((board->nextColour == white) ? board->blackPieces : board->whitePieces) | (uint64_t)1 << board->enPassantSquare;

	//The bitboard of all pawns that are not pinned. (pinned pieces need to be calculated seperatly)
	uint64_t pawnBitboard = board->getPieceBitboard(board->nextColour, pawn) & ~pinnedPieces;
	uint64_t pinnedPawnBitboard = board->getPieceBitboard(board->nextColour, pawn) & pinnedPieces;

	//Adds en-passant position to capture moves (if not pinned)
	if (board->enPassantSquare != -1)
	{
		const uint64_t enemyEnPassantTarget = captureMask & shift((uint64_t)1 << board->enPassantSquare,-forwards);
		if (enemyEnPassantTarget)
		{
			//The two pieces that will be (re)moved by an enpassant capture
			uint64_t leftPos = (uint64_t)1 << (board->enPassantSquare - forwards - 1) & ~fileH & board->getPieceBitboard(board->nextColour, pawn);
			uint64_t rightPos = (uint64_t)1 << (board->enPassantSquare - forwards + 1) & ~fileA & board->getPieceBitboard(board->nextColour, pawn);

			if (leftPos && rightPos)
			{
				captureMask |= (uint64_t)1 << board->enPassantSquare;
			}
			else if (leftPos && !isPinnedEnPassant(board, leftPos | enemyEnPassantTarget))
			{
				captureMask |= (uint64_t)1 << board->enPassantSquare;
			}
			else if (rightPos && !isPinnedEnPassant(board, rightPos | enemyEnPassantTarget))
			{
				captureMask |= (uint64_t)1 << board->enPassantSquare;
			}
			else
			{
				captureMask &= ~((uint64_t)1 << board->enPassantSquare);
			}
		}
	}


	//Adds quiet moves to moveList
	pawnMoves = shift(pawnBitboard, forwards) & emptySquares;
	pawnDoubleMoves= shift(pawnMoves & rank3BB, forwards) & emptySquares & pushMask;
	pawnMoves &= pushMask;

	while (pawnMoves)
	{
		uint64_t currentMove = pop(pawnMoves);
		int to = bitScanForward(currentMove);
		arraySize = addPawnMovesPromotions(to - forwards, to, currentMove, quietMove, board, Movelist, arraySize);
	}
	while (pawnDoubleMoves)
	{
		uint64_t currentMove = pop(pawnDoubleMoves);
		int to = bitScanForward(currentMove);
		(*Movelist)[arraySize] = Move(to - forwards * 2, to, quietMove, pawn, board);
		arraySize++;
	}

	//Adds captures moves to moveList. (right and left attacks are handled seperatly)
	pawnAttacks = shift(pawnBitboard, rightAttack) & enemyPieces & ~fileA & captureMask;
	while (pawnAttacks)
	{
		uint64_t currentMove = pop(pawnAttacks);
		int to = bitScanForward(currentMove);
		arraySize = addPawnMovesPromotions(to - rightAttack, to, currentMove, capture, board, Movelist, arraySize);

	}
	pawnAttacks = shift(pawnBitboard, leftAttack) & enemyPieces & ~fileH & captureMask;
	while (pawnAttacks)
	{
		uint64_t currentMove = pop(pawnAttacks);
		int to = bitScanForward(currentMove);
		arraySize = addPawnMovesPromotions(to - leftAttack, to, currentMove, capture, board, Movelist, arraySize);
	}


	while (pinnedPawnBitboard)
	{
		pawnPos = pop(pinnedPawnBitboard);
		legalMoves = generateLegalFilterForPinnedPiece(board, pawnPos);

		int pawnPosIndex = bitScanForward(pawnPos);

		pawnMoves = shift(pawnPos, forwards) & ~board->allPieces & legalMoves; //Move forward
		pawnDoubleMoves = shift(pawnMoves & rank3BB,  forwards) & ~board->allPieces & legalMoves & pushMask; //Move twice on first turn if first is clear
		pawnMoves &= pushMask;

		if (pawnDoubleMoves)
		{
			int to = bitScanForward(pawnDoubleMoves);
			(*Movelist)[arraySize] = Move(to - forwards * 2, to, quietMove, pawn, board);
			arraySize++;
		}

		pawnAttacks = ((board->nextColour == white) ? pawnWhiteAttacksArray[pawnPosIndex] : pawnBlackAttacksArray[pawnPosIndex]) & enemyPieces;
		pawnAttacks &= legalMoves & captureMask;

		arraySize = addPawnMoves(pawnPosIndex, pawnMoves, pawnAttacks, board, Movelist, arraySize);
	}

	return arraySize;
}

int generateKingMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares, int arraySize)
{
	uint64_t kingBitboard;
	if (board->nextColour == white)
	{
		kingBitboard = board->getPieceBitboard(white, king);
	}
	else
	{
		kingBitboard = board->getPieceBitboard(black, king);
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
	uint64_t knightBitboard = board->getPieceBitboard(board->nextColour, knight) & ~pinnedPieces;
	
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
		rookBitboard = board->getPieceBitboard(white, rook);
	}
	else
	{
		rookBitboard = board->getPieceBitboard(black, rook);
	}
	while (rookBitboard)
	{
		uint64_t currentRook = pop(rookBitboard);
		int currentPos = bitScanForward(currentRook);

		if (currentRook & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentRook);
		else legalMoves = ~0;

		uint64_t moves = rookMoves(currentPos, board->allPieces) & ~friendlyPieces & legalMoves & (pushMask | captureMask);

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
		bishopBitboard = board->getPieceBitboard(white, bishop);
	}
	else
	{
		bishopBitboard = board->getPieceBitboard(black, bishop);
	}
	while (bishopBitboard)
	{
		uint64_t currentBishop = pop(bishopBitboard);
		int currentPos = bitScanForward(currentBishop);

		if (currentBishop & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentBishop);
		else legalMoves = ~0;

		uint64_t moves = bishopMoves(currentPos, board->allPieces) & ~friendlyPieces & legalMoves & (pushMask | captureMask);

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
		queenBitboard = board->getPieceBitboard(white, queen);
	}
	else
	{
		queenBitboard = board->getPieceBitboard(black, queen);
	}
	while (queenBitboard)
	{
		uint64_t currentQueen = pop(queenBitboard);
		int currentPos = bitScanForward(currentQueen);

		if (currentQueen & pinnedPieces) legalMoves = generateLegalFilterForPinnedPiece(board, currentQueen);
		else legalMoves = ~0;

		uint64_t moves = queenMoves(currentPos, board->allPieces) & ~friendlyPieces & legalMoves & (pushMask | captureMask);

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

int addPawnMovesPromotions(int from, int to, uint64_t move, MoveType type, Board* board, std::array<Move, 150>* Movelist, int arraySize)
{
	if ((board->nextColour == white && (move & rank8)) || (board->nextColour == black && (move & rank1)))
	{
		(*Movelist)[arraySize] = Move(from, to, rookPromotion, pawn, board);
		(*Movelist)[arraySize + 1] = Move(from, to, knightPromotion, pawn, board);
		(*Movelist)[arraySize + 2] = Move(from, to, queenPromotion, pawn, board);
		(*Movelist)[arraySize + 3] = Move(from, to, bishopPromotion, pawn, board);
		arraySize += 4;
	}
	else
	{
		(*Movelist)[arraySize] = Move(from, to, type, pawn, board);
		arraySize++;
	}
	return arraySize;
}

uint64_t getPinnedPieces(Board * board)
{
	uint64_t pinnedPieces = 0;

	uint64_t kingBitBoard = board->getPieceBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	const uint64_t enemyPieces = (board->nextColour == white) ? board->blackPieces : board->whitePieces;
	const uint64_t friendlyPieces = (board->nextColour == white) ? board->whitePieces : board->blackPieces;

	uint64_t kingRaysRook = rookMoves(kingPos, enemyPieces) & (board->getPieceBitboard(switchColour(board->nextColour), rook) | board->getPieceBitboard(switchColour(board->nextColour), queen));
	while (kingRaysRook)
	{
		uint64_t pinner = pop(kingRaysRook);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & friendlyPieces;
		if (bitSum(pinnedPiece) == 1) pinnedPieces |= pinnedPiece;
	}

	uint64_t kingRaysBishop = bishopMoves(kingPos, enemyPieces) & (board->getPieceBitboard(switchColour(board->nextColour), bishop) | board->getPieceBitboard(switchColour(board->nextColour), queen));;
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
	uint64_t kingBitBoard = board->getPieceBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	uint64_t currentPos, currentRay;
	uint64_t rookBitboard = board->getPieceBitboard(switchColour(board->nextColour), rook) | board->getPieceBitboard(switchColour(board->nextColour), queen);
	while (rookBitboard)
	{
		currentPos = pop(rookBitboard);
		int piecePos = bitScanForward(currentPos);

		if (inBetween(kingPos, piecePos) & pinnedPiece)
		{
			return inBetween(kingPos, piecePos) | currentPos;
		}
		
	}

	uint64_t bishopBitboard = board->getPieceBitboard(switchColour(board->nextColour), bishop) | board->getPieceBitboard(switchColour(board->nextColour), queen);
	while (bishopBitboard)
	{
		currentPos = pop(bishopBitboard);
		int piecePos = bitScanForward(currentPos);

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
		attackers |= board->getPieceBitboard(opponentColour, pawn) & (((targetBitboard << 7) & ~fileH) | ((targetBitboard << 9) & ~fileA));
	}
	else
	{
		attackers |= board->getPieceBitboard(opponentColour, pawn) & (((targetBitboard >> 9) & ~fileH) | ((targetBitboard >> 7) & ~fileA));
	}

	//KnightMoves
	attackers |= knightMovesArray[targetPos] & board->getPieceBitboard(opponentColour, knight);

	//KingMoves
	attackers |= kingMovesArray[targetPos] & board->getPieceBitboard(opponentColour, king);
	
	//Rook and half of queen moves
	attackers |= rookMoves(targetPos, board->allPieces) & (board->getPieceBitboard(opponentColour, rook) | (board->getPieceBitboard(opponentColour, queen)));
	
	//Bishop and half of queen moves
	attackers |= bishopMoves(targetPos, board->allPieces) & (board->getPieceBitboard(opponentColour, bishop) | (board->getPieceBitboard(opponentColour, queen)));

	return attackers;
}

bool isPinnedEnPassant(Board* board, uint64_t pieces)
{
	uint64_t kingBitBoard = board->getPieceBitboard(board->nextColour, king);
	int kingPos = bitScanForward(kingBitBoard);

	uint64_t kingRaysRook = rookMoves(kingPos, 0) & (board->getPieceBitboard(switchColour(board->nextColour), rook) | board->getPieceBitboard(switchColour(board->nextColour), queen));
	while (kingRaysRook)
	{
		uint64_t pinner = pop(kingRaysRook);
		uint64_t pinnedPiece = inBetween(kingPos, bitScanForward(pinner)) & board->allPieces;
		if (pinnedPiece ==  pieces) return true;
	}

	return false;
}
