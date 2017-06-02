#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

#include "pieceSquare.h"
#include "board.h"
#include "piece.h"
#include "moveTree.h"
#include "moveGeneration.h"
#include "scoring.h"
#include "move.h"
#include "utils.h"


void engineLoop();
std::vector<std::string> split(std::string words);
void setupEngine();


