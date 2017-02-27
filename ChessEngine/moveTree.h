#pragma once
#include <vector>
#include <array>
#include <minmax.h>

#include "piece.h"
#include "moveGeneration.h"
#include "scoring.h"

struct moveTreeNode
{
	std::array<std::array<piece, 8>, 8> move;
	int score , alpha , beta;
	int recursionLayers;
	void fillTree(colours aiColour , colours nodeColour , bool first);
	std::vector<moveTreeNode> children;
};
