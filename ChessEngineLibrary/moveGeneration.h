#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "move.h"
#include "board.h"
#include "magicBitboards.h"

std::vector<Move> searchForMoves(Board* board);

void generatePawnMoves(Board* board, std::vector<Move>& Movelist, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask);
void generateKingMoves(Board* board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares);
void generateKnightMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask);
void generateRookMoves(Board* board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask);
void generateBishopMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask);
void generateQueenMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t pinnedPieces, uint64_t pushMask, uint64_t captureMask);
void generateCastlingMoves(Board * board, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, uint64_t kingDangerSquares);

void addPawnMoves(int start, uint64_t quietMoves, uint64_t captureMoves, Board* board, std::vector<Move>& Movelist);
void addMoves(int start, int end, pieceType piece, std::vector<Move>& Movelist, uint64_t enemyPieces, Board* board);

uint64_t generateAttackSet(Board* board, colours colour, uint64_t allPieces);
uint64_t getPinnedPieces(Board* board);
uint64_t generateLegalFilterForPinnedPiece(Board* board, uint64_t pinnedPiece);
uint64_t getAttackers(Board* board, colours colour, uint64_t targetBitboard);
bool isPinnedEnPassant(Board* board, uint64_t pieces);
void setupMoveGen();
