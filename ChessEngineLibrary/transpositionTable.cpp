#include "transpositionTable.h"


void ZorbistKeys::initialize()
{
	srand(25461);

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			pieceKeys[i][j] = get64rand();
		}
	}
	blackMoveKey = get64rand();
}

int getZorbistKey(Board * board, colours colour)
{
	board->update();
	int hash = 0;
	for (int x = 0; x < 64; x++)
	{
		//std::cout << "current hash " << (((uint64_t)1 << x) & board->allPieces) << "\n";
		if ((((uint64_t)1 << x) & board->allPieces) > 0) //If their is a piece at the square.
		{
			//std::cout << "current hash " << ((uint64_t)1 << x) << "\n";
			if (board->blackPawnBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black pawn
			{
				hash ^= ZorbistKeys::pieceKeys[x][0];
			}
			else if (board->blackKnightBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black knight
			{
				hash ^= ZorbistKeys::pieceKeys[x][1];
			}
			else if (board->blackBishopBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black bishop
			{
				hash ^= ZorbistKeys::pieceKeys[x][2];
			}
			else if (board->blackRookBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black rook
			{
				hash ^= ZorbistKeys::pieceKeys[x][3];
			}
			else if (board->blackQueenBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black queen
			{
				hash ^= ZorbistKeys::pieceKeys[x][4];
			}
			else if (board->blackKingBitboard & ((uint64_t)1 << x) > 0) //If the piece is a black king
			{
				hash ^= ZorbistKeys::pieceKeys[x][5];
			}
			else if (board->whitePawnBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white pawn
			{
				hash ^= ZorbistKeys::pieceKeys[x][6];
			}
			else if (board->whiteKnightBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white knight
			{
				hash ^= ZorbistKeys::pieceKeys[x][7];
			}
			else if (board->whiteBishopBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white bishop
			{
				hash ^= ZorbistKeys::pieceKeys[x][8];
			}
			else if (board->whiteRookBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white rook
			{
				hash ^= ZorbistKeys::pieceKeys[x][9];
			}
			else if (board->whiteQueenBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white queen
			{
				hash ^= ZorbistKeys::pieceKeys[x][10];
			}
			else if (board->whiteKingBitboard & ((uint64_t)1 << x) > 0) //If the piece is a white king
			{
				hash ^= ZorbistKeys::pieceKeys[x][11];
			}
		}
	}
	if (colour == black)
	{
		hash ^= ZorbistKeys::blackMoveKey;
	}
	return hash;
}

int updateHash(Move newMove, int oldHash, colours colour) //currently only works for quiet moves.
{
	int newHash = oldHash ^ ZorbistKeys::blackMoveKey;
	newHash ^= ZorbistKeys::pieceKeys[newMove.from][newMove.piece + 6 * colour];
	newHash ^= ZorbistKeys::pieceKeys[newMove.to][newMove.piece + 6 * colour];
	return newHash;
}

uint64_t get64rand() {
	return
		(((uint64_t)rand() << 0) & 0x000000000000FFFFull) |
		(((uint64_t)rand() << 16) & 0x00000000FFFF0000ull) |
		(((uint64_t)rand() << 32) & 0x0000FFFF00000000ull) |
		(((uint64_t)rand() << 48) & 0xFFFF000000000000ull);
}

int ZorbistKeys::pieceKeys[64][12];
int ZorbistKeys::blackMoveKey;
std::unordered_map<int, TranspositionEntry> ZorbistKeys::TranspositionTable;