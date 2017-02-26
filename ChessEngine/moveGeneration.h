#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"

std::vector<std::array<std::array<piece, 8>, 8>> searchForMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour);

std::vector<std::array<std::array<piece, 8>, 8>> generatePawnMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour , int x, int y);
std::vector<std::array<std::array<piece, 8>, 8>> generateKnightMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y);
std::vector<std::array<std::array<piece, 8>, 8>> generateKingMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y);
std::vector<std::array<std::array<piece, 8>, 8>> generateBishopMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y);
std::vector<std::array<std::array<piece, 8>, 8>> generateRookMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y);