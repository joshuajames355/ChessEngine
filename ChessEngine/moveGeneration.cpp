#include "stdafx.h"
#include "moveGeneration.h"

std::vector<std::array<std::array<piece, 8>, 8>> searchForMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList , newMoves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y].colour == aiColour) //If the piece is of the right colour
			{
				switch (board[x][y].type)
				{
				case pawn:
					{
						newMoves = generatePawnMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				case knight:
					{
						newMoves = generateKnightMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				case bishop:
					{
						newMoves = generateBishopMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				case rook:
					{
						newMoves = generateRookMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				case queen:
					{
						newMoves = generateBishopMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
						newMoves = generateRookMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				case king:
					{
						newMoves = generateKingMoves(board, aiColour, x, y);
						moveList.insert(moveList.end(), newMoves.begin(), newMoves.end());
					}
					break;

				}
			}
		}
	}
	return moveList;
}

std::vector<std::array<std::array<piece, 8>, 8>> generatePawnMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList;
	if (aiColour == black) //Pawns move in different directions for each team
	{
		if (y != 7)
		{
			if (board[x][y + 1].type == blank) //Performs move forward
			{
				if (y == 1 && board[x][y + 2].type == blank)//Can move twice on first move.
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x][y + 2] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
				std::array<std::array<piece, 8>, 8> newMove = board;
				newMove[x][y + 1] = newMove[x][y];
				newMove[x][y].type = blank;
				moveList.push_back(newMove);
			}
			if (x != 0) //Capturing to the left
			{
				if (board[x - 1][y + 1].type != blank && board[x - 1][y + 1].colour != aiColour)
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x - 1][y + 1] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
			}
			if (x != 7) //Capturing to the right
			{
				if (board[x + 1][y + 1].type != blank && board[x + 1][y + 1].colour != aiColour)
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x + 1][y + 1] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
			}
		}
	}
	else //Movement For White
	{
		if (y != 0)
		{
			if (board[x][y - 1].type == blank) //Performs move forward
			{
				if (y == 6 && board[x][y - 2].type == blank)//Can move twice on first move.
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x][y - 2] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
				std::array<std::array<piece, 8>, 8> newMove = board;
				newMove[x][y - 1] = newMove[x][y];
				newMove[x][y].type = blank;
				moveList.push_back(newMove);
			}
			if (x > 0) //Capturing to the left
			{
				if (board[x - 1][y - 1].type != blank && board[x - 1][y - 1].colour != aiColour)
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x - 1][y - 1] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
			}
			if (x < 7) //Capturing to the right
			{
				if (board[x + 1][y - 1].type != blank && board[x + 1][y - 1].colour != aiColour)
				{
					std::array<std::array<piece, 8>, 8> newMove = board;
					newMove[x + 1][y - 1] = newMove[x][y];
					newMove[x][y].type = blank;
					moveList.push_back(newMove);
				}
			}
		}
	}
	return moveList;
}

std::vector<std::array<std::array<piece, 8>, 8>> generateKnightMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList;
	std::array<std::array<int, 2>, 8> offsets = { {
		{ 1,2 },{ 2,1 },
		{ -1,2 },{ -2,1 },
		{ 1,-2 },{ 2,-1 },
		{ -1,-2 },{ -2,-1 } } };
	for (int offset = 0; offset < offsets.size(); offset++)
	{
		if (x + offsets[offset][0] >= 0 && x + offsets[offset][0] <= 7 && y + offsets[offset][1] >= 0 && y + offsets[offset][1] <= 7) //Checks that the offset applied to the piece is in bounds
		{

			if (board[x + offsets[offset][0]][y + offsets[offset][1]].colour != aiColour || board[x + offsets[offset][0]][y + offsets[offset][1]].type == blank) //Checks the position is blank or a different colour
			{
				std::array<std::array<piece, 8>, 8> newMove = board;
				newMove[x + offsets[offset][0]][y + offsets[offset][1]] = newMove[x][y];
				newMove[x][y].type = blank;
				moveList.push_back(newMove);
			}
		}
	}
	return moveList;
}

std::vector<std::array<std::array<piece, 8>, 8>> generateKingMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList;
	std::array<std::array<int, 2>, 8> offsets = { {
		{ 0,1 },{ 1,1 },
		{ 1,0 },{ 1,-1 },
		{ 0,-1 },{ -1,-1 },
		{ -1,0 },{ -1,1 }} };
	for (int offset = 0; offset < offsets.size(); offset++)
	{
		if (x + offsets[offset][0] >= 0 && x + offsets[offset][0] <= 7 && y + offsets[offset][1] >= 0 && y + offsets[offset][1] <= 7) //Checks that the offset applied to the piece is in bounds
		{

			if (board[x + offsets[offset][0]][y + offsets[offset][1]].colour != aiColour || board[x + offsets[offset][0]][y + offsets[offset][1]].type == blank) //Checks the position is blank or a different colour
			{
				std::array<std::array<piece, 8>, 8> newMove = board;
				newMove[x + offsets[offset][0]][y + offsets[offset][1]] = newMove[x][y];
				newMove[x][y].type = blank;
				moveList.push_back(newMove);
			}
		}
	}
	return moveList;
}

std::vector<std::array<std::array<piece, 8>, 8>> generateBishopMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList;

	for (int counter = 1; x + counter <= 7 && y + counter <= 7; counter++) //Move to the top-right
	{
		if (board[x + counter][y + counter].type == blank)
		{
			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x + counter][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x + counter][y + counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x + counter][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; x - counter >= 0 && y + counter <= 7; counter++) //Move to the top-let
	{
		if (board[x - counter][y + counter].type == blank)
		{
			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x - counter][y + counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; x - counter >= 0 && y - counter >= 0; counter++) //Move to the bottom-left
	{
		if (board[x - counter][y - counter].type == blank)
		{
			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x - counter][y - counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; x + counter <= 7 && y - counter >= 0; counter++) //Move to the bottom-right
	{
		if (board[x + counter][y - counter].type == blank)
		{
			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x + counter][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x + counter][y - counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x + counter][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	return moveList;
}

std::vector<std::array<std::array<piece, 8>, 8>> generateRookMoves(std::array<std::array<piece, 8>, 8> board, colours aiColour, int x, int y)
{
	std::vector<std::array<std::array<piece, 8>, 8>> moveList;

	for (int counter = 1; x + counter <= 7; counter++) //Move to the right
	{
		if (board[x + counter][y].type == blank)
		{
			std::array<std::array<piece, 8>, 8> newMove = board; 
			newMove[x + counter][y] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x + counter][y].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x + counter][y] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; x - counter >= 0; counter++) //Move to the left
	{
		if (board[x - counter][y].type == blank)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x - counter][y].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x - counter][y] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; y + counter <= 7; counter++) //Move down
	{
		if (board[x][y + counter].type == blank)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[counter][y + counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x][y + counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	for (int counter = 1; y - counter >= 0; counter++) //Move up
	{
		if (board[x][y - counter].type == blank)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
		}
		else if (board[x][y - counter].colour != aiColour)
		{

			std::array<std::array<piece, 8>, 8> newMove = board;
			newMove[x][y - counter] = newMove[x][y];
			newMove[x][y].type = blank;
			moveList.push_back(newMove);
			break;
		}
		else
		{
			break;
		}
	}

	return moveList;
}
