#pragma once
#include <vector>
#include <array>
#include <algorithm>

#include "piece.h"
#include "moveGeneration.h"
#include "board.h"
#include "scoring.h"
#include "move.h"

struct moveTreeNode
{
	Board move;
	Move moveRaw;
	int score , alpha , beta;
	int recursionLayers;
	void fillTree(colours aiColour , colours nodeColour , bool first);
	std::vector<moveTreeNode> children;
};
