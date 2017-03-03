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
}

void Board::defaults()
{
	whitePawnBitboard = rank1;
	whiteRookBitboard = 129; //2^0 + 2^7
	whiteKnightBitboard = 66; //2^1 + 2^6
	whiteBishopBitboard = 36; //2^2 + 2^5
	whiteQueenBitboard = 8; //2^3
	whiteKingBitboard = 16; //2^4

	blackPawnBitboard = rank6;
	blackRookBitboard = 9295429630892703744; //2^56 + 2^63
	blackKnightBitboard = 4755801206503243776; //2^57 + 2^62
	blackBishopBitboard = 2594073385365405696; //2^58 + 2^61
	blackQueenBitboard = 576460752303423488; //2^59
	blackKingBitboard = 1152921504606846976; //2^60
}

void Board::printBoard()
{
	int counter = 56;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{

			int64_t currentPosBitboard = (int64_t)1 << counter;

			//Checking for white pieces
			if ((whitePawnBitboard & currentPosBitboard) != 0) //If the piece is a white pawn;
			{
				std::cout << "|" << "WP";
			}
			else if ((whiteRookBitboard & currentPosBitboard) != 0) //If the piece is a white rook;
			{
				std::cout << "|" << "WR";
			}
			else if ((whiteKnightBitboard & currentPosBitboard) != 0) //If the piece is a white knight;
			{
				std::cout << "|" << "WN";
			}
			else if ((whiteBishopBitboard & currentPosBitboard) != 0) //If the piece is a white bishop;
			{
				std::cout << "|" << "WB";
			}
			else if ((whiteQueenBitboard & currentPosBitboard) != 0) //If the piece is a white queen;
			{
				std::cout << "|" << "WQ";
			}
			else if ((whiteKingBitboard & currentPosBitboard) != 0) //If the piece is a white king;
			{
				std::cout << "|" << "WK";
			}

			//Checking for black pieces
			else if ((blackPawnBitboard & currentPosBitboard) != 0) //If the piece is a black pawn;
			{
				std::cout << "|" << "BP";
			}
			else if ((blackRookBitboard & currentPosBitboard) != 0) //If the piece is a black rook;
			{
				std::cout << "|" << "BR";
			}
			else if ((blackKnightBitboard & currentPosBitboard) != 0) //If the piece is a black knight;
			{
				std::cout << "|" << "BN";
			}
			else if ((blackBishopBitboard & currentPosBitboard) != 0) //If the piece is a black bishop;
			{
				std::cout << "|" << "BB";
			}
			else if ((blackQueenBitboard & currentPosBitboard) != 0) //If the piece is a black queen;
			{
				std::cout << "|" << "BQ";
			}
			else if ((blackKingBitboard & currentPosBitboard) != 0) //If the piece is a black king;
			{
				std::cout << "|" << "BK";
			}
			else
			{
				std::cout << "|" << "  ";
			}
			counter++;
		}
		std::cout << "|\n";
		counter -= 16;
	}
}

int bitSum(int64_t bitboard)
{
	int count = 0;
	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}
	return count;
}
