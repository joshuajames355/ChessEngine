#include "moveTree.h"

void moveTreeNode::fillTree(colours aiColour, colours nodeColour , bool first)
{
	//Ai colour is the colour of this node , newcolour is the colour of child nodes.
	colours newColour;
	switch (nodeColour)
	{
	case white:
		newColour = black;
		break;
	case black:
		newColour = white;
		break;
	}

	if (recursionLayers == 0)
	{
		//score = calculateScoreDiff(move, aiColour);
		return;
	}

	if (aiColour == nodeColour) //If maximizer
	{
		/*
		std::vector<std::array<std::array<piece, 8>, 8>> moves = searchForMoves(move, nodeColour);
		score = -10000;
		for (int x = 0; x < moves.size(); x++)
		{
			moveTreeNode newChild;
			newChild.move = moves[x];
			newChild.recursionLayers = recursionLayers - 1;
			newChild.alpha = alpha;
			newChild.beta = beta;
			if (first)
			{
				children.push_back(newChild);
				children[children.size() - 1].fillTree(aiColour, newColour, false);
				score = std::max(score, children[children.size() - 1].score);
				alpha = std::max(alpha, score);
			}
			else
			{
				newChild.fillTree(aiColour, newColour, false);
				score = std::max(score, newChild.score);
				alpha = std::max(alpha, score);
			}

			if (beta <= alpha)
			{
				return;
			}
		}
	}
	else // If minimizer
	{
		std::vector<std::array<std::array<piece, 8>, 8>> moves = searchForMoves(move, nodeColour);
		score = 10000;
		for (int x = 0; x < moves.size(); x++)
		{
			moveTreeNode newChild;
			newChild.move = moves[x];
			newChild.recursionLayers = recursionLayers - 1;
			newChild.alpha = alpha;
			newChild.beta = beta;
			if (first)
			{
				children.push_back(newChild);
				children[children.size() - 1].fillTree(aiColour, newColour, false);
				score = std::min(score, children[children.size() - 1].score);
				beta = std::min(beta, score);
			}
			else
			{
				newChild.fillTree(aiColour, newColour, false);
				score = std::min(score, newChild.score);
				beta = std::min(beta, score);
			}

			if (beta <= alpha)
			{
				return;
			}
		}
		*/
	}
}
