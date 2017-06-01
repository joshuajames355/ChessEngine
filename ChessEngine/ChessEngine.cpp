// ChessEngine.cpp : Defines the entry point for the console application.
//

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
	magicBitboards::setupMagicBitboards();
	engineLoop();
}

void engineLoop()
{

	int SEARCHDEPTH = 4;


	Board board = Board();
	board.defaults();

	colours aiColour = white;
	while (true)
	{
		std::string response;
		std::getline(std::cin, response);
		std::vector<std::string> words = split(response);

		if (response == "uci")
		{
			std::cout << "id name UNSR Chess System\n";
			std::cout << "id author UNSR\n";
			std::cout << "option name SearchDepth type spin default 4 min 2 max 20\n";
			std::cout << "uciok\n";
		}
		if (response == "isready")
		{
			std::cout << "readyok\n";
		}
		if (response == "quit")
		{
			break;
		}
		if (words[0] == "setoption" && words[1] == "name")
		{
			if (words[2] == "SearchDepth" && words[3] == "value")
			{
				SEARCHDEPTH = std::stoi(words[4]);
			}
		}

		if (words[0] == "position" && words[1] == "startpos")
		{
			if (words.size() == 2)
			{
				std::cout << "test";
				board.defaults();
				aiColour = white;
			}
			else if (words[2] == "moves")
			{
				board.defaults();

				aiColour = white;
				Move currentMove;
				for (int x = 3; x < words.size(); x++)
				{
					currentMove = moveFromNotation(words[x], &board);
					std::cout << "from: " << currentMove.from << " to: " << currentMove.to << " type: "  << currentMove.moveType << "\n";
					board = currentMove.applyMove(&board, aiColour);
					board.update();
					switch (aiColour)
					{
					case white:
						aiColour = black;
						break;
					case black:
						aiColour = white;
						break;
					}
				}
			}
		}

		if (words[0] == "go")
		{
			moveTreeNode moveTree;
			moveTree.move = board;
			moveTree.alpha = -9999999;
			moveTree.beta = 9999999;

			switch (aiColour)
			{
			case white:
				std::cout << "White\n";
				break;
			case black:
				std::cout << "Black\n";
				break;
			}

			moveTree.recursionLayers = SEARCHDEPTH;
			board.printBoard();

			moveTree.fillTree(aiColour, aiColour, true);

			int bestScore = -999999999;
			Move bestMove;
			for (int x = 0; x < moveTree.children.size(); x++)
			{
				if (moveTree.children[x].score > bestScore)
				{
					bestMove = moveTree.children[x].moveRaw;
					bestScore = moveTree.children[x].score;
				}
			}


			std::cout << "\nbestmove " << notationFromMove(bestMove) << "\n";
		}
	}

}

std::vector<std::string> split(std::string words)
{
	std::string temp = "";
	std::vector<std::string> wordList;
	for (int x = 0; x < words.size(); x++)
	{
		if (words[x] == ' ')
		{
			if (temp != "")
			{
				wordList.push_back(temp);
				temp = "";
			}
		}
		else
		{
			temp += words[x];
		}
		if (x == words.size() - 1)
		{
			wordList.push_back(temp);
		}

	}
	return wordList;
}

