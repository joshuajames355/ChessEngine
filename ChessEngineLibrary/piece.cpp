#include "piece.h"

piece::piece()
{
	return;
}

piece::piece(std::string val)
{
	if (val[0] == 'W')
	{
		colour = white;
	}
	else if (val[0] == 'B')
	{
		colour = black;
	}
	else
	{
		type = blank;
		colour = black;
		return;
	}

	if (val[1] == 'P')
	{
		type = pawn;
	}
	else if (val[1] == 'N')
	{
		type = knight;
	}
	else if (val[1] == 'B')
	{
		type = bishop;
	}
	else if (val[1] == 'R')
	{
		type = rook;
	}
	else if (val[1] == 'Q')
	{
		type = queen;
	}
	else if (val[1] == 'K')
	{
		type = king;
	}
	else
	{
		type = blank;
		colour = black;
	}
}

std::string piece::toString()
{
	std::string name;
	if (colour == black)
	{
		name += "B";
	}
	else
	{
		name += "W";
	}

	if (type == pawn)
	{
		name += "P";
	}
	else if (type == knight)
	{
		name += "N";
	}
	else if (type == bishop)
	{
		name += "B";
	}
	else if (type == rook)
	{
		name += "R";
	}
	else if (type == queen)
	{
		name += "Q";
	}
	else if (type == king)
	{
		name += "K";
	}
	else if (type == blank)
	{
		name = "  ";
	}
	return name;
}

colours switchColour(colours colour)
{
	switch (colour)
	{
	case white:
		return black;
	case black:
		return white;
	}	
}

