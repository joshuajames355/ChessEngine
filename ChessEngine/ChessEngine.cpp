// ChessEngine.cpp : Defines the entry point for the console application.
//

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
	engineLoop();
}

void engineLoop()
{
	TranspositionEntry* transpositionTable = new TranspositionEntry[TTSize];

	Board board = Board();
	board.defaults();
	bool hasSetupEngine = false;

	while (true)
	{
		std::string response;
		std::getline(std::cin, response);
		std::vector<std::string> words = split(response);

		if (response == "uci")
		{
			std::cout << "id name UNSR Chess System\n";
			std::cout << "id author UNSR\n";
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

		if (words[0] == "position")
		{
			if (words[1] == "startpos")
			{
				if (words.size() == 2)
				{
					board.defaults();
				}
				else if (words[2] == "moves")
				{
					board.defaults();
					Move currentMove;
					for (int x = 3; x < words.size(); x++)
					{
						//Applies each move to calculate the current board.
						currentMove = moveFromNotation(words[x], &board);
						currentMove.applyMove(&board);
					}
				}
			}
			else if (words[1] == "fen")
			{
				std::string fenString = response.substr(13);
				board = Board();
				board.loadFromFen(fenString);

				if (words.size() > 8 && words[8] == "moves") {
					Move currentMove;
					for (int x = 9; x < words.size(); x++)
					{
						//Applies each move to calculate the current board.
						currentMove = moveFromNotation(words[x], &board);
						currentMove.applyMove(&board);

					}
				}
			}
		}

		if (words[0] == "go")
		{
			//Default value of 15 mins
			long int btime = 900000;
			long int wtime = 900000;
			int depth = -1;

			//Retreives the clock values from the message
			if (std::find(words.begin(), words.end(), "btime") != words.end())
			{
				btime = std::stoi(*(std::find(words.begin(), words.end(), "btime") + 1));
			}
			if (std::find(words.begin(), words.end(), "wtime") != words.end())
			{
				wtime = std::stoi(*(std::find(words.begin(), words.end(), "wtime") + 1));
			}

			//Retreives depth from message , if set
			if (std::find(words.begin(), words.end(), "depth") != words.end())
			{
				depth = std::stoi(*(std::find(words.begin(), words.end(), "depth") + 1));
			}

			std::cout << btime << " " << wtime << "\n";

			timeManagement timer;
			if (depth == -1)
				timer = timeManagement(btime, wtime, board.nextColour);
			else
				timer = timeManagement(depth);

			board.printBoard();

			Move pv = startSearch(&board, transpositionTable, &timer);

			std::cout << "bestmove " << notationFromMove(pv) << "\n";
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
	setupMoveGen();
	setupBitboardUtils();
}
