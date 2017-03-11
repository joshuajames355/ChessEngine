#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "move.h"
#include "board.h"
#include "magicBitboards.h"

std::vector<Move> searchForMoves(Board* board, colours aiColour);

void generatePawnMoves(Board* board ,colours aiColour, std::vector<Move>& Movelist);
void generateKingMoves(Board* board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateKnightMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces);
void generateRookMoves(Board* board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces , magicBitboards* magicData);
void generateBishopMoves(Board * board, colours aiColour, std::vector<Move>& Movelist, uint64_t friendlyPieces, uint64_t enemyPieces, magicBitboards* magicData);

void addMoves(int start, int end, pieceType piece , std::vector<Move>& Movelist , uint64_t enemyPieces);
