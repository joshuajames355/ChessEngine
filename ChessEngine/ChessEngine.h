#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "board.h"
#include "search.h"
#include "utils.h"
#include "transpositionTable.h"


void engineLoop();
std::vector<std::string> split(std::string words);
void setupEngine();


