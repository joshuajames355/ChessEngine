// ChessEngine.cpp : Defines the entry point for the console application.
//

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
	engineLoop();
}

void engineLoop()
{

	int SEARCHDEPTH = 4;


	Board board = Board();
	board.defaults();
	bool hasSetupEngine = false;

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
			if (!hasSetupEngine)
			{
				hasSetupEngine = true;
				setupEngine();
			}
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

		if (words[0] == "position")
		{
			if (words[1] == "startpos")
			{
				if (words.size() == 2)
				{
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
						//Applies each move to calculate the current board.
						currentMove = moveFromNotation(words[x], &board);
						std::cout << "from: " << currentMove.from << " to: " << currentMove.to << " type: " << currentMove.moveType << "\n";
						board = currentMove.applyMove(&board, aiColour);
						board.update();

						//Calculate the colour of the AI. Even number of moves = white , odd number of moves = black
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
			else if (words[1] == "fen")
			{
				std::string fenString = words[2] + " " + words[3] + " " + words[4] + " " + words[5] + " " + words[6] + " " + words[7];
				board = Board();
				aiColour = board.loadFromFen(fenString);

				if (words.size() > 8 && words[8] == "moves") {
					Move currentMove;
					for (int x = 9; x < words.size(); x++)
					{
						//Applies each move to calculate the current board.
						currentMove = moveFromNotation(words[x], &board);
						std::cout << "from: " << currentMove.from << " to: " << currentMove.to << " type: " << currentMove.moveType << "\n";
						board = currentMove.applyMove(&board, aiColour);
						board.update();

						//Calculate the colour of the AI. Even number of moves = white , odd number of moves = black
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
		}

		if (words[0] == "go")
		{
			board.printBoard();
			std::cout << "bestmove " << notationFromMove(startSearch(SEARCHDEPTH, board, aiColour)) << "\n";
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

void setupEngine()
{
	magicBitboards::setupMagicBitboards();
	ZorbistKeys::initialize();
}
