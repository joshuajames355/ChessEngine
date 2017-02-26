#include "stdafx.h"
#include "moveTree.h"

void moveTreeNode::fillTree(colours aiColour)
{
	colours newColour;
	switch (aiColour)
	{
	case white:
		newColour = black;
		break;
	case black:
		newColour = white;
		break;
	}
	std::vector<std::array<std::array<piece, 8>, 8>> moves = searchForMoves(move, newColour);
	for (int x = 0; x < moves.size(); x++)
	{
		moveTreeNode newChild;
		newChild.move = moves[x];
		newChild.score = calculateScoreDiff(moves[x] , newColour);
		newChild.recursionLayers = recursionLayers - 1;
		children.push_back(newChild);
		if (children[children.size() - 1].recursionLayers > 0)
		{
			children[children.size() - 1].fillTree(newColour);
		}
	}
}
