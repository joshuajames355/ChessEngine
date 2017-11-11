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
#include "pieceSquare.h"

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

	//Checks whether the game is drawn by lack of material
	bool isMaterialDraw();

	//Gets the colour of a piece on the board
	colours  getPieceColour(int pos);

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

	//returns total positional score (not including kings)
	int getPositionalScore(colours colour);
	//returns the current positional score for the king (mid game)
	int getMidGameKingPositionalScore(colours colour);
	//returns the current positional score for the king (late game)
	int getLateGameKingPositionalScore(colours colour);

	//updates the values for material and positional scores.
	//Only run once , values are updated incrementally from then on.
	void updateScoreValues();

	//Adds the material value of a piece.
	void addMaterialScore(colours colour, pieceType piece);
	//Removes the material value of the piece .
	void removeMaterialScore(colours colour, pieceType piece);

	//Adds the material value of a piece.
	void addPositionalScore(colours colour, pieceType piece, int piecePos);
	//Removes the material value of the piece .
	void removePositionalScore(colours colour, pieceType piece, int piecePos);

	//Material score not including pawns. (used for phase changed in scoring.cpp)
	int whiteMaterialScore;
	int blackMaterialScore;

	//Material score of pawns
	int whitePawnScore;
	int blackPawnScore;

	//Positional scores based on piece-square tables.
	int whitePositionalScore;
	int blackPositionalScore;

	//Positional scores of kings. (usage changes based on game phase)
	int whiteMidGameKingPositionalScore;
	int blackMidGameKingPositionalScore;
	int whiteLateGameKingPositionalScore;
	int blackLateGameKingPositionalScore;

	inline void resetFiftyMoveTimer() { fiftyMoveTimer = 0; };
	inline void incrementFiftyMoveTimer() { fiftyMoveTimer++; };
	inline int getFiftyMoveTimer() { return fiftyMoveTimer; };
	inline int setFiftyMoveTimer(int newTimer) { fiftyMoveTimer = newTimer; };

private:
	uint64_t pieceBitboards[2][6];
	int fiftyMoveTimer;
};