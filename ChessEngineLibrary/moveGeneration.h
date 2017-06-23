#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "move.h"
#include "board.h"
#include "magicBitboards.h"

std::vector<Move> searchForMoves(Board* board);

void generatePawnMoves(Board* board, std::vector<Move>& Movelist);
void generateKingMoves(Board* board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateKnightMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateRookMoves(Board* board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateBishopMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateQueenMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateCastlingMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);

void addPawnMoves(int start, uint64_t quietMoves, uint64_t captureMoves, Board* board, std::vector<Move>& Movelist);
void addMoves(int start, int end, pieceType piece, std::vector<Move>& Movelist, uint64_t enemyPieces, Board* board);
