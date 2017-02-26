#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>

#include "piece.h"


class moveTable
{

public:
	moveTable();
	moveTable(std::string filename, pieceType typeNew, bool mustFlip = false);
	void loadFromFile(std::string filename);
	std::vector<std::array<int,2>> moves;
	pieceType type;
};