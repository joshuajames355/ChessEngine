// ChessEngine.cpp : Defines the entry point for the console application.
//

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
	engineLoop();

	/*
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

	std::cout << "\n" << bestScore << "\n" << findNameDifference(board, bestMove);


	int test;
	std::cin >> test;
	
	*/
}

void engineLoop()
{
	std::array<std::array<piece, 8>, 8> board ={ {
	{ piece("BR"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WR") },
	{ piece("BN"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WN") },
	{ piece("BB"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WB") },
	{ piece("BK"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WK") },
	{ piece("BQ"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WQ") },
	{ piece("BB"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WB") },
	{ piece("BN"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WN") },
	{ piece("BR"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WR") }} };

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
		if (words[0] == "position" && words[1] == "startpos" && words[2] == "moves")
		{
			board = { {
				{ piece("BR"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WR") },
				{ piece("BN"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WN") },
				{ piece("BB"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WB") },
				{ piece("BK"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WK") },
				{ piece("BQ"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WQ") },
				{ piece("BB"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WB") },
				{ piece("BN"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WN") },
				{ piece("BR"),piece("BP"),piece("  ") ,piece("  ") ,piece("  ") ,piece("  ") ,piece("WP") ,piece("WR") } } };

			for (int x = 3; x < words.size(); x++)
			{
				board = applyMove(board, words[x]);
			}
			if ((words.size() - 3)%2 == 0)
			{
				std::cout << "white\n";
				aiColour = white;
			}
			else
			{
				std::cout << "black\n";
				aiColour = black;
			}
			printBoard(board);
		}

		if (words[0] == "go")
		{
			moveTreeNode moveTree;
			moveTree.move = board;
			moveTree.alpha = -9999999;
			moveTree.beta = 9999999;

			moveTree.recursionLayers = 5;

			moveTree.fillTree(aiColour, aiColour, true);

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

			printBoard(bestMove);

			std::cout << "\nbestmove " << findNameDifference(board, bestMove , aiColour) << "\n";
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

std::array<std::array<piece, 8>, 8> applyMove(std::array<std::array<piece, 8>, 8> board, std::string uciCommand)
{
	int x1 = uciCommand[0] - 'a';
	int y1 = 8 - (uciCommand[1] - '0');
	int x2 = uciCommand[2] - 'a';
	int y2 = 8 - (uciCommand[3] - '0');

	board[x2][y2] = board[x1][y1];
	board[x1][y1].type = blank;
	return board; 
}

std::string findNameDifference(std::array<std::array<piece, 8>, 8> board1, std::array<std::array<piece, 8>, 8> board2, colours aiColour)
{
	int x1, x2, y1, y2;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board1[x][y].type == board2[x][y].type && board1[x][y].colour == board2[x][y].colour)
			{
				//Match
			}
			else if (board1[x][y].colour == aiColour)
			{
				if (board1[x][y].type == blank) //Moved from this piece
				{
					x2 = x;
					y2 = y;
				}
				else //Moved to this piece
				{
					x1 = x;
					y1 = y;
				}
			}
		}
	}

	
	const char charArray[5] = { x1 + 97,8 - y1 + '0' , x2 + 97 ,8 - y2 +'0',0};
	return  std::string(charArray);
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
