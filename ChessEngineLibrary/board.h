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
	Board(std::string fenString);

	void clearBoard();
	void defaults();
	void printBoard();
	void update();
	void nextMove();

	void loadFromFen(std::string fen);
	std::string exportAsFen();

	uint64_t getPieceBitboard(colours colour , pieceType piece);
	void setBitboard(colours colour, pieceType piece, uint64_t bitboard);
	//returns the value of the piece lost.
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

	//Used for for indexing the main transposition table.
	uint64_t zorbistKey;

	//Used for indexing pawn structure scores hash table.
	uint64_t pawnScoreZorbistKey;

	uint64_t whitePieces;
	uint64_t blackPieces;
	uint64_t allPieces;


	uint64_t kingDangerSquares;

	bool isInCheck();
	uint64_t getKingDangerSquares();
	void generateKingDangerSquares();

	//returns the total material score.
	int getMaterialScore(colours colour);
	//returns the material score (no pawns) , for only one colour.
	int getOnlyMaterialScore(colours colour);
	void updateMaterialScore();
	//Adds the value of a piece
	void addMaterialScore(colours colour, pieceType piece);
	//Removes the value of a piece.
	void removeMaterialScore(colours colour, pieceType piece);

private:
	uint64_t pieceBitboards[2][6];

	//Score not including pawns. (used for phase changed in scoring.cpp)
	int whiteMaterialScore;
	int blackMaterialScore;

	int whitePawnScore;
	int blackPawnScore;
};