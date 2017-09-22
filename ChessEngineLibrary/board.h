#pragma once
#include <stdint.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "bitboard.h"
#include "magicBitboards.h"
#include "piece.h"
#include "transpositionTable.h"
#include "moveGenerationTables.h"

class Board
{
public:
	Board();
	void clearBoard();
	void defaults();
	void printBoard();
	void update();
	void nextMove();

	void loadFromFen(std::string fen);
	std::string exportAsFen();

	uint64_t findBitboard(colours colour , pieceType piece);
	void setBitboard(colours colour, pieceType piece, uint64_t bitboard);
	void removePiece(uint64_t bitboard);
	pieceType getPieceTypeInSquare(uint64_t bitboard);
	bool isPieceAttacked(int piecePos, colours colour);

	void generateZorbistKey();

	//Used for detecting three-fold repitition.
	std::vector<uint64_t> moveHistory;

	int enPassantSquare;
	colours nextColour;

	bool canBlackCastleQueenSide;
	bool canBlackCastleKingSide;
	bool canWhiteCastleQueenSide;
	bool canWhiteCastleKingSide;

	uint64_t zorbistKey;

	uint64_t whitePawnBitboard;
	uint64_t whiteKnightBitboard;
	uint64_t whiteBishopBitboard;
	uint64_t whiteRookBitboard;
	uint64_t whiteQueenBitboard;
	uint64_t whiteKingBitboard;
	
	uint64_t blackPawnBitboard;
	uint64_t blackKnightBitboard;
	uint64_t blackBishopBitboard;
	uint64_t blackRookBitboard;
	uint64_t blackQueenBitboard;
	uint64_t blackKingBitboard;

	uint64_t whitePieces;
	uint64_t blackPieces;
	uint64_t allPieces;

	bool isInCheck();
	uint64_t getKingDangerSquares();
	void generateKingDangerSquares();

	//This will either setup for or undo a null move
	void doNullMove();
	void undoNullMove();

	uint64_t kingDangerSquares;
};