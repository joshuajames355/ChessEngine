#pragma once
#include <vector>
#include <algorithm>

#include "move.h"
#include "moveGenerationTables.h"
#include "board.h"

class killerEntry
{
public:
	killerEntry();
	std::vector<Move> getKillerMoves() { return killerMoves; };
	void addKillerMove(Move move);

private:
	std::vector<Move> killerMoves;
	int numKillers;
};

struct SEEPiece
{
	SEEPiece() {};
	SEEPiece(uint64_t newPieceBitboard, pieceType newPieceType);

	uint64_t pieceBitboard;
	pieceType type;

	bool operator==(const SEEPiece& b)
	{
		return (b.pieceBitboard == pieceBitboard) && (b.type == type);
	}
};

void orderSearch(std::array<Move, 150>* moveList, Board* board, int arraySize, Move* TTMove, bool isBestMove, killerEntry killerMoves);
int orderQuiescentSearch(std::array<Move, 150>* moveList, Board * board, int arraySize);

int getMVVLVAScore(Move* move);
bool MVVLVAComparisonFunc(Move move1, Move move2);
void MVVLVA(std::array<Move, 150>* moveList, Board * board, int arraySize);

int SEE(Move* move, Board* board);
void SEEAddSliders(std::array<SEEPiece, 20>* whiteAttackers, std::array<SEEPiece, 20>* blackAttackers, Board* board, int targetSquare, uint64_t occupancyMask, int* whiteAttackersArraySize, int* blackAttackersArraySize);
SEEPiece getLeastValuablePiece(std::array<SEEPiece, 20>* attackers, int* arraySize);

