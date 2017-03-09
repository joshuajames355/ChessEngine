#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "move.h"
#include "board.h"

std::vector<Move> searchForMoves(Board* board, colours aiColour);

void generatePawnMoves(Board* board ,colours aiColour, std::vector<Move>& Movelist);
void generateKingMoves(Board* board, colours aiColour, std::vector<Move>& Movelist);
void generateKnightMoves(Board * board, colours aiColour, std::vector<Move>& Movelist)

void addMoves(int start, int end, colours aiColour, pieceType piece , std::vector<Move>& Movelist , Board* board);
