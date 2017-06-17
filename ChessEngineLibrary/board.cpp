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
	update();

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

	std::string castlingAvaliability;
	while (!isspace(currentChar))
	{
		castlingAvaliability += currentChar;
		currentCharPos++;
		currentChar = fen[currentCharPos];
	}

	//Load castling Avaliability

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

int bitSum(uint64_t bitboard)
{
	int count = 0;
	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}
	return count;
}

uint64_t pop(uint64_t& bitboard)
{
	uint64_t lsb = bitboard & -bitboard;
	bitboard -= lsb;
	return lsb;
}

const int index64[64] = {
	0,  1, 48,  2, 57, 49, 28,  3,
	61, 58, 50, 42, 38, 29, 17,  4,
	62, 55, 59, 36, 53, 51, 43, 22,
	45, 39, 33, 30, 24, 18, 12,  5,
	63, 47, 56, 27, 60, 41, 37, 16,
	54, 35, 52, 21, 44, 32, 23, 11,
	46, 26, 40, 15, 34, 20, 31, 10,
	25, 14, 19,  9, 13,  8,  7,  6
};

int bitScanForward(uint64_t bitboard) {
	const uint64_t debruijn64 = 285870213051386505;
	return index64[((bitboard & -bitboard) * debruijn64) >> 58];
}

std::vector<int> getSetBits(uint64_t bitboard)
{
	std::vector<int> setBits;
	while (bitboard)
	{
		uint64_t pos = pop(bitboard);
		setBits.push_back(bitScanForward(pos));
	}
	return setBits;
}
