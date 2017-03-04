#pragma once
#include <vector>
#include <array>
#include <iostream>

#include "piece.h"
#include "board.h"

std::vector<Board> searchForMoves(Board* board, colours aiColour);

void generatePawnMoves(Board* board ,colours aiColour, std::vector<Board>& Movelist);
