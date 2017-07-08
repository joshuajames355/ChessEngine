#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "board.h"
#include "search.h"
#include "utils.h"
#include "transpositionTable.h"
#include "moveGeneration.h"
#include "timeManagement.h"
#include "utils.h"


void engineLoop();
std::vector<std::string> split(std::string words);
void setupEngine();


