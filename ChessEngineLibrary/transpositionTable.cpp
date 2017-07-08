#include "transpositionTable.h"


void ZorbistKeys::initialize()
{
	srand(25461);

	//The key's for the hash keys are generated as pieceType + colour*6. 
	//In order to read polygot values, these must be converted as follow's.
	std::unordered_map<int, int> polygotPieceTypeConversion;
	polygotPieceTypeConversion[0]  = 1;
	polygotPieceTypeConversion[1]  = 3;
	polygotPieceTypeConversion[2]  = 5;
	polygotPieceTypeConversion[3]  = 7;
	polygotPieceTypeConversion[4]  = 9;
	polygotPieceTypeConversion[5]  = 11;
	polygotPieceTypeConversion[6]  = 0;
	polygotPieceTypeConversion[7]  = 2;
	polygotPieceTypeConversion[8]  = 4;
	polygotPieceTypeConversion[9]  = 6;
	polygotPieceTypeConversion[10] = 8;
	polygotPieceTypeConversion[11] = 10;


	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			pieceKeys[i][j] = PolygotPieceKeys[64 * polygotPieceTypeConversion[j] + i];
		}
	}
	for (int i = 0; i < 8; i++)
	{
		enPassantKeys[i] = PolygotPieceKeys[772 + i];
	}

	whiteMoveKey = PolygotPieceKeys[780];
	blackQueenSideCastlingKey = PolygotPieceKeys[771];
	blackKingSideCastlingKey = PolygotPieceKeys[770];
	whiteQueenSideCastlingKey = PolygotPieceKeys[769];
	whiteKingSideCastlingKey = PolygotPieceKeys[768];
}

uint64_t get64rand() {
	return
		(((uint64_t)rand() << 0) & 0x000000000000FFFFull) |
		(((uint64_t)rand() << 16) & 0x00000000FFFF0000ull) |
		(((uint64_t)rand() << 32) & 0x0000FFFF00000000ull) |
		(((uint64_t)rand() << 48) & 0xFFFF000000000000ull);
}

uint64_t ZorbistKeys::pieceKeys[64][12];
uint64_t ZorbistKeys::whiteMoveKey;
uint64_t ZorbistKeys::blackQueenSideCastlingKey;
uint64_t ZorbistKeys::blackKingSideCastlingKey;
uint64_t ZorbistKeys::whiteQueenSideCastlingKey;
uint64_t ZorbistKeys::whiteKingSideCastlingKey;
uint64_t ZorbistKeys::enPassantKeys[8];