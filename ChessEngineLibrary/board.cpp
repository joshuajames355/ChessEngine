#include "board.h"


Board::Board()
{
	whitePawnBitboard = emptyBitboard;
	whiteRookBitboard = emptyBitboard; 
	whiteKnightBitboard = emptyBitboard; 
	whiteBishopBitboard = emptyBitboard; 
	whiteQueenBitboard = emptyBitboard; 
	whiteKingBitboard = emptyBitboard; 

	blackPawnBitboard = emptyBitboard;
	blackRookBitboard = emptyBitboard; 
	blackKnightBitboard = emptyBitboard;
	blackBishopBitboard = emptyBitboard;
	blackQueenBitboard = emptyBitboard; 
	blackKingBitboard = emptyBitboard; 

	nextColour = white;
	enPassantSquare = -1;

	canBlackCastleQueenSide = false;
	canBlackCastleKingSide = false;
	canWhiteCastleQueenSide = false;
	canWhiteCastleKingSide = false;

	update();
}

void Board::defaults()
{
	whitePawnBitboard = rank2;
	whiteRookBitboard = 129; //2^0 + 2^7
	whiteKnightBitboard = 66; //2^1 + 2^6
	whiteBishopBitboard = 36; //2^2 + 2^5
	whiteQueenBitboard = 8; //2^3
	whiteKingBitboard = 16; //2^4

	blackPawnBitboard = rank7;
	blackRookBitboard = 9295429630892703744; //2^56 + 2^63
	blackKnightBitboard = 4755801206503243776; //2^57 + 2^62
	blackBishopBitboard = 2594073385365405696; //2^58 + 2^61
	blackQueenBitboard = 576460752303423488; //2^59
	blackKingBitboard = 1152921504606846976; //2^60

	nextColour = white;
	enPassantSquare = -1;

	canBlackCastleQueenSide = true;
	canBlackCastleKingSide = true;
	canWhiteCastleQueenSide = true;
	canWhiteCastleKingSide = true;

	update();
}

void Board::printBoard()
{
	int counter = 56;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{

			uint64_t currentPosBitboard = (uint64_t)1 << counter;

			std::cout << "|";

			//Checking for white pieces
			if ((whitePawnBitboard & currentPosBitboard) != 0) //If the piece is a white pawn;
			{
				std::cout << "WP";
			}
			if ((whiteRookBitboard & currentPosBitboard) != 0) //If the piece is a white rook;
			{
				std::cout <<  "WR";
			}
			if ((whiteKnightBitboard & currentPosBitboard) != 0) //If the piece is a white knight;
			{
				std::cout << "WN";
			}
			if ((whiteBishopBitboard & currentPosBitboard) != 0) //If the piece is a white bishop;
			{
				std::cout << "WB";
			}
			if ((whiteQueenBitboard & currentPosBitboard) != 0) //If the piece is a white queen;
			{
				std::cout << "WQ";
			}
			if ((whiteKingBitboard & currentPosBitboard) != 0) //If the piece is a white king;
			{
				std::cout << "WK";
			}

			//Checking for black pieces
			if ((blackPawnBitboard & currentPosBitboard) != 0) //If the piece is a black pawn;
			{
				std::cout << "BP";
			}
			if ((blackRookBitboard & currentPosBitboard) != 0) //If the piece is a black rook;
			{
				std::cout << "BR";
			}
			if ((blackKnightBitboard & currentPosBitboard) != 0) //If the piece is a black knight;
			{
				std::cout << "BN";
			}
			if ((blackBishopBitboard & currentPosBitboard) != 0) //If the piece is a black bishop;
			{
				std::cout << "BB";
			}
			if ((blackQueenBitboard & currentPosBitboard) != 0) //If the piece is a black queen;
			{
				std::cout << "BQ";
			}
			if ((blackKingBitboard & currentPosBitboard) != 0) //If the piece is a black king;
			{
				std::cout << "BK";
			}
			if ((allPieces & currentPosBitboard) == 0) //If the piece is empty
			{
				std::cout << "  ";
			}
			counter++;
		}
		std::cout << "|\n";
		counter -= 16;
	}
}

void Board::update()
{
	blackPieces = blackPawnBitboard | blackRookBitboard | blackKnightBitboard | blackBishopBitboard | blackKingBitboard | blackQueenBitboard;
	whitePieces = whitePawnBitboard | whiteRookBitboard | whiteKnightBitboard | whiteBishopBitboard | whiteKingBitboard | whiteQueenBitboard;

	allPieces = whitePieces | blackPieces;
}

void Board::loadFromFen(std::string fen)
{
	int currentPosInBoard = 56;
	int currentCharPos = 0;
	char currentChar = 'X';

	while (!isspace(currentChar))
	{
		currentChar = fen[currentCharPos];
		if (currentChar == 'p')
		{
			blackPawnBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'r')
		{
			blackRookBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'n')
		{
			blackKnightBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'b')
		{
			blackBishopBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'k')
		{
			blackKingBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'q')
		{
			blackQueenBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'P')
		{
			whitePawnBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'R')
		{
			whiteRookBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'N')
		{
			whiteKnightBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'B')
		{
			whiteBishopBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'K')
		{
			whiteKingBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'Q')
		{
			whiteQueenBitboard |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == '/')
		{
			currentPosInBoard -= 16;
		}
		else
		{
			currentPosInBoard += currentChar - '0';
		}
		currentCharPos++;
	}

	if (fen[currentCharPos] == 'w')
	{
		nextColour = white;
	}
	else if (fen[currentCharPos] == 'b')
	{
		nextColour = black;
	}

	currentCharPos += 2;
	currentChar = fen[currentCharPos];

	while (!isspace(currentChar))
	{
		if (currentChar == 'K')
		{
			canWhiteCastleKingSide = true;
		}
		else if (currentChar == 'Q')
		{
			canWhiteCastleQueenSide = true;
		}
		else if (currentChar == 'k')
		{
			canBlackCastleKingSide = true;
		}
		else if (currentChar == 'q')
		{
			canBlackCastleQueenSide = true;
		}
		currentCharPos++;
		currentChar = fen[currentCharPos];
	}


	currentCharPos++;
	currentChar = fen[currentCharPos];
	if (currentChar != '-')
	{
		//En passant target square
		int column = currentChar - 'a';
		int row = fen[currentCharPos + 1] - '1';
		int pos = row * 8 + column;

		enPassantSquare = pos;
	}
	else
	{
		enPassantSquare = -1;
	}

	currentCharPos += 2;

	//Halfmove clock
	//Fullmove clock	

	update();

}

uint64_t Board::findBitboard(colours colour, pieceType piece)
{
	if (colour == white)
	{
		switch (piece)
		{
		case pawn:
			return whitePawnBitboard;
		case knight:
			return whiteKnightBitboard;
		case bishop:
			return whiteBishopBitboard;
		case rook:
			return whiteRookBitboard;
		case queen:
			return whiteQueenBitboard;
		case king:
			return whiteKingBitboard;
		}
	}
	else
	{
		switch (piece)
		{
		case pawn:
			return blackPawnBitboard;
		case knight:
			return blackKnightBitboard;
		case bishop:
			return blackBishopBitboard;
		case rook:
			return blackRookBitboard;
		case queen:
			return blackQueenBitboard;
		case king:
			return blackKingBitboard;
		}
	}
}

void Board::setBitboard(colours colour, pieceType piece, uint64_t bitboard)
{
	if (colour == white)
	{
		switch (piece)
		{
		case pawn:
			whitePawnBitboard = bitboard;
			break;
		case knight:
			whiteKnightBitboard = bitboard;
			break;
		case bishop:
			whiteBishopBitboard = bitboard;
			break;
		case rook:
			whiteRookBitboard = bitboard;
			break;
		case queen:
			whiteQueenBitboard = bitboard;
			break;
		case king:
			whiteKingBitboard = bitboard;
			break;
		}
	}
	else
	{
		switch (piece)
		{
		case pawn:
			blackPawnBitboard = bitboard;
			break;
		case knight:
			blackKnightBitboard = bitboard;
			break;
		case bishop:
			blackBishopBitboard = bitboard;
			break;
		case rook:
			blackRookBitboard = bitboard;
			break;
		case queen:
			blackQueenBitboard = bitboard;
			break;
		case king:
			blackKingBitboard = bitboard;
			break;
		}
	}
}

void Board::removePiece(uint64_t bitboard)
{
	if ((whitePawnBitboard & bitboard) != 0)
	{
		whitePawnBitboard = whitePawnBitboard & ~bitboard;
		return;
	}
	if ((whiteKnightBitboard & bitboard) != 0)
	{
		whiteKnightBitboard = whiteKnightBitboard & ~bitboard;
		return;
	}
	if ((whiteBishopBitboard & bitboard) != 0)
	{
		whiteBishopBitboard = whiteBishopBitboard & ~bitboard;
		return;
	}
	if ((whiteRookBitboard & bitboard) != 0)
	{
		whiteRookBitboard = whiteRookBitboard & ~bitboard;
		return;
	}
	if ((whiteQueenBitboard & bitboard) != 0)
	{
		whiteQueenBitboard = whiteQueenBitboard & ~bitboard;
		return;
	}
	if ((whiteKingBitboard & bitboard) != 0)
	{
		whiteKingBitboard = whiteKingBitboard & ~bitboard;
		return;
	}



	if ((blackPawnBitboard & bitboard) != 0)
	{
		blackPawnBitboard = blackPawnBitboard & ~bitboard;
		return;
	}
	if ((blackKnightBitboard & bitboard) != 0)
	{
		blackKnightBitboard = blackKnightBitboard & ~bitboard;
		return;
	}
	if ((blackBishopBitboard & bitboard) != 0)
	{
		blackBishopBitboard = blackBishopBitboard & ~bitboard;
		return;
	}
	if ((blackRookBitboard & bitboard) != 0)
	{
		blackRookBitboard = blackRookBitboard & ~bitboard;
		return;
	}
	if ((blackQueenBitboard & bitboard) != 0)
	{
		blackQueenBitboard = blackQueenBitboard & ~bitboard;
		return;
	}
	if ((blackKingBitboard & bitboard) != 0)
	{
		blackKingBitboard = blackKingBitboard & ~bitboard;
		return;
	}
}

bool Board::isPieceAttacked(int piecePos, colours colour)
{
	uint64_t pieceBitboard = (uint64_t)1 << piecePos;

	if (colour == white)
	{
		if ((pieceBitboard << 7) & blackPawnBitboard & ~fileH || (pieceBitboard << 9) & blackPawnBitboard & ~fileA)
		{
			return true;
		}
	}
	else
	{
		if ((pieceBitboard >> 9) & whitePawnBitboard & ~fileH || (pieceBitboard >> 7) & whitePawnBitboard & ~fileA)
		{
			return true;
		}
	}

	//KnightMoves
	uint64_t knightMoves= (pieceBitboard << 15) & ~fileH; //2 up 1 left
	knightMoves |= (pieceBitboard << 17) & ~fileA; //2 up 1 Right
	knightMoves |= (pieceBitboard << 06) & ~fileH & ~fileG;//1 up 2 left
	knightMoves |= (pieceBitboard << 10) & ~fileA & ~fileB;//1 up 2 right
	knightMoves |= (pieceBitboard >> 10) & ~fileH & ~fileG;//1 down 2 left
	knightMoves |= (pieceBitboard >> 06) & ~fileA & ~fileB;//1 down 2 right
	knightMoves |= (pieceBitboard >> 17) & ~fileH;//2 down 1 left
	knightMoves |= (pieceBitboard >> 15) & ~fileA;//2 down 1 right

	if (colour == white)
	{
		if (knightMoves & blackKnightBitboard)
		{
			return true;
		}
	}
	else
	{
		if (knightMoves & whiteKnightBitboard)
		{
			return true;
		}
	}

	uint64_t moves = pieceBitboard << 8;
	moves |= pieceBitboard << 9 & ~fileA;
	moves |= pieceBitboard << 1 & ~fileA;
	moves |= pieceBitboard >> 7 & ~fileA;
	moves |= pieceBitboard >> 8;
	moves |= pieceBitboard >> 9 & ~fileH;
	moves |= pieceBitboard >> 1 & ~fileH;
	moves |= pieceBitboard << 7 & ~fileH;

	if (colour == white)
	{
		if (knightMoves & blackKingBitboard)
		{
			return true;
		}
	}
	else
	{
		if (knightMoves & whiteKingBitboard)
		{
			return true;
		}
	}
	
	//Rook and half of queen moves
	uint64_t occupancy = magicBitboards::rookMask[piecePos] & allPieces;
	uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[piecePos];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[piecePos];
	uint64_t magicMoves = magicBitboards::magicMovesRook[piecePos][arrayIndex];

	if (colour == white)
	{
		if (magicMoves & (blackRookBitboard | blackQueenBitboard))
		{
			return true;
		}
	}
	else
	{
		if (magicMoves & (whiteRookBitboard | whiteQueenBitboard))
		{
			return true;
		}
	}

	//Bishop and half of queen moves
	occupancy = magicBitboards::bishopMask[piecePos] & allPieces;
	magicResult = occupancy * magicBitboards::magicNumberBishop[piecePos];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[piecePos];
	magicMoves = magicBitboards::magicMovesBishop[piecePos][arrayIndex];

	if (colour == white)
	{
		if (magicMoves & (blackBishopBitboard | blackQueenBitboard))
		{
			return true;
		}
	}
	else
	{
		if (magicMoves & (whiteBishopBitboard | whiteQueenBitboard))
		{
			return true;
		}
	}
	return false;
}
