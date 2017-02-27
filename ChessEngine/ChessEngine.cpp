// ChessEngine.cpp : Defines the entry point for the console application.
//

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
	const int SEARCHDEPTH = 4;

	if (argc < 65)
	{
		std::cout << "Not Enough Arguments \n";
		return 0;
	}

	//Imports board
	int counter = 1;
	std::array<std::array<piece, 8>, 8> board;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			board[x][y] = piece(std::string(argv[counter]));
			counter++;
		}
	}

	printBoard(board);
	std::cout << "\n\n";

	//Loads in colour of AI
	colours aiColour;
	if (std::string(argv[65]) == "white")
	{
		aiColour = white;
	}
	else if(std::string(argv[65]) == "black")
	{
		aiColour = black;
	}
	else
	{
		aiColour = white;
	}

	moveTreeNode moveTree;
	moveTree.move = board;
	moveTree.alpha = -9999999;
	moveTree.beta = 9999999;

	moveTree.recursionLayers = SEARCHDEPTH;

	time_t timer = time(NULL);

	moveTree.fillTree(aiColour , aiColour,true);

	int bestScore = -999999999;
	std::array<std::array<piece, 8>, 8> bestMove;
	for (int x = 0; x < moveTree.children.size(); x++)
	{
		if (moveTree.children[x].score > bestScore)
		{
			bestMove = moveTree.children[x].move;
			bestScore = moveTree.children[x].score;
		}
	}

	std::cout << "\n" << difftime(time(NULL), timer) << "\n";
	std::cout << "Done\n\n\n\n";

	printBoard(bestMove);

	std::cout << "\n" << bestScore << "\n";


	int test;
	std::cin >> test;

}

void printBoard(std::array<std::array<piece, 8>, 8> board)
{
	for (int y = 0; y < 8; y++)
	{
		std::cout << " | ";
		for (int x = 0; x < 8; x++)
		{
			std::cout << board[x][y].toString() << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}
