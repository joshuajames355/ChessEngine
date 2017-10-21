#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "move.h"
#include "board.h"
#include "magicBitboards.h"
#include "moveGenerationTables.h"

int searchForMoves(Board* board, std::array<Move,150>* moveList);

int generatePawnMoves(Board* board, std::array<Move, 150>* Movelist, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize);
int generateKingMoves(Board* board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares, int arraySize);
int generateKnightMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize);
int generateRookMoves(Board* board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize);
int generateBishopMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize);
int generateQueenMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask, int arraySize);
int generateCastlingMoves(Board * board, std::array<Move, 150>* Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares, int arraySize);

int addPawnMoves(int start, uint64_t quietMoves, uint64_t captureMoves, Board* board, std::array<Move, 150>*, int arraySize);
int addMoves(int start, int end, pieceType piece, std::array<Move, 150>*, uint64_t enemyPieces, Board* board, int arraySize);

uint64_t generateAttackSet(Board* board, colours colour, uint64_t allPieces);
uint64_t getPinnedPieces(Board* board);
uint64_t generateLegalFilterForPinnedPiece(Board* board, uint64_t pinnedPiece);
uint64_t getAttackers(Board* board, colours colour, uint64_t targetBitboard);
bool isPinnedEnPassant(Board* board, uint64_t pieces);
