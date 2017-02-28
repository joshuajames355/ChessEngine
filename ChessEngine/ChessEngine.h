#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

#include "pieceSquare.h"
#include "piece.h"
#include "moveTree.h"
#include "moveGeneration.h"
#include "scoring.h"

void printBoard(std::array<std::array<piece, 8>, 8> board);
void engineLoop();
std::vector<std::string> split(std::string words);

std::array<std::array<piece, 8>, 8> applyMove(std::array<std::array<piece, 8>, 8> board,std::string uciCommand);
std::string findNameDifference(std::array<std::array<piece, 8>, 8> board1, std::array<std::array<piece, 8>, 8> board2, colours aiColour);

