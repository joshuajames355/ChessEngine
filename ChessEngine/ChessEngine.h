#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "pieceSquare.h"
#include "piece.h"
#include "moveTree.h"
#include "moveGeneration.h"
#include "scoring.h"

void printBoard(std::array<std::array<piece, 8>, 8> board);