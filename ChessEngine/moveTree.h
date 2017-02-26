#pragma once
#include <vector>
#include <array>

#include "piece.h"
#include "moveGeneration.h"
#include "scoring.h"

struct moveTreeNode
{
	std::array<std::array<piece, 8>, 8> move;
	int score;
	std::vector<moveTreeNode> children;
	int recursionLayers;
	void fillTree(colours aiColour);
};
