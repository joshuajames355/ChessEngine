#pragma once
#include <string>
#include <iostream>

enum pieceType { pawn, knight, bishop, rook, queen, king, blank };
enum colours : bool { white = true, black = false };

colours switchColour(colours colour);

class piece
{
public:
	pieceType type;
	colours colour;
	piece();
	piece(std::string val);
	std::string toString();

};