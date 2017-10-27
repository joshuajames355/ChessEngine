#include "board.h"


Board::Board()
{
	clearBoard();
}

Board::Board(std::string fenString)
{
	loadFromFen(fenString);
}

void Board::clearBoard()
{
	pieceBitboards[white][pawn] = emptyBitboard;
	pieceBitboards[white][rook] = emptyBitboard;
	pieceBitboards[white][knight] = emptyBitboard;
	pieceBitboards[white][bishop] = emptyBitboard;
	pieceBitboards[white][queen] = emptyBitboard;
	pieceBitboards[white][king] = emptyBitboard;

	pieceBitboards[black][pawn] = emptyBitboard;
	pieceBitboards[black][rook] = emptyBitboard;
	pieceBitboards[black][knight] = emptyBitboard;
	pieceBitboards[black][bishop] = emptyBitboard;
	pieceBitboards[black][queen] = emptyBitboard;
	pieceBitboards[black][king] = emptyBitboard;

	nextColour = white;
	enPassantSquare = -1;

	canBlackCastleQueenSide = false;
	canBlackCastleKingSide = false;
	canWhiteCastleQueenSide = false;
	canWhiteCastleKingSide = false;

	kingDangerSquares = 0;
	whiteMaterialScore = 0;
	blackMaterialScore = 0;

	zorbistKey = 0;
	pawnScoreZorbistKey = 0;

	update();
}

void Board::defaults()
{
	pieceBitboards[white][pawn] = rank2;
	pieceBitboards[white][rook] = 129; //2^0 + 2^7
	pieceBitboards[white][knight] = 66; //2^1 + 2^6
	pieceBitboards[white][bishop] = 36; //2^2 + 2^5
	pieceBitboards[white][queen] = 8; //2^3
	pieceBitboards[white][king] = 16; //2^4

	pieceBitboards[black][pawn] = rank7;
	pieceBitboards[black][rook] = 9295429630892703744; //2^56 + 2^63
	pieceBitboards[black][knight] = 4755801206503243776; //2^57 + 2^62
	pieceBitboards[black][bishop] = 2594073385365405696; //2^58 + 2^61
	pieceBitboards[black][queen] = 576460752303423488; //2^59
	pieceBitboards[black][king] = 1152921504606846976; //2^60

	nextColour = white;
	enPassantSquare = -1;

	canBlackCastleQueenSide = true;
	canBlackCastleKingSide = true;
	canWhiteCastleQueenSide = true;
	canWhiteCastleKingSide = true;

	generateZorbistKey();
	updateScoreValues();
	update();
}

void Board::printBoard()
{
	int counter = 56;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{

			uint64_t currentPosBitboard = (uint64_t)1 << counter;

			std::cout << "|";

			//Checking for white pieces
			if ((pieceBitboards[white][pawn] & currentPosBitboard) != 0) //If the piece is a white pawn;
			{
				std::cout << "WP";
			}
			if ((pieceBitboards[white][rook] & currentPosBitboard) != 0) //If the piece is a white rook;
			{
				std::cout <<  "WR";
			}
			if ((pieceBitboards[white][knight] & currentPosBitboard) != 0) //If the piece is a white knight;
			{
				std::cout << "WN";
			}
			if ((pieceBitboards[white][bishop] & currentPosBitboard) != 0) //If the piece is a white bishop;
			{
				std::cout << "WB";
			}
			if ((pieceBitboards[white][queen] & currentPosBitboard) != 0) //If the piece is a white queen;
			{
				std::cout << "WQ";
			}
			if ((pieceBitboards[white][king] & currentPosBitboard) != 0) //If the piece is a white king;
			{
				std::cout << "WK";
			}

			//Checking for black pieces
			if ((pieceBitboards[black][pawn] & currentPosBitboard) != 0) //If the piece is a black pawn;
			{
				std::cout << "BP";
			}
			if ((pieceBitboards[black][rook] & currentPosBitboard) != 0) //If the piece is a black rook;
			{
				std::cout << "BR";
			}
			if ((pieceBitboards[black][knight] & currentPosBitboard) != 0) //If the piece is a black knight;
			{
				std::cout << "BN";
			}
			if ((pieceBitboards[black][bishop] & currentPosBitboard) != 0) //If the piece is a black bishop;
			{
				std::cout << "BB";
			}
			if ((pieceBitboards[black][queen] & currentPosBitboard) != 0) //If the piece is a black queen;
			{
				std::cout << "BQ";
			}
			if ((pieceBitboards[black][king] & currentPosBitboard) != 0) //If the piece is a black king;
			{
				std::cout << "BK";
			}
			if ((allPieces & currentPosBitboard) == 0) //If the piece is empty
			{
				std::cout << "  ";
			}
			counter++;
		}
		std::cout << "|\n";
		counter -= 16;
	}
}

void Board::update()
{
	blackPieces = pieceBitboards[black][pawn] | pieceBitboards[black][rook] | pieceBitboards[black][knight] | pieceBitboards[black][bishop] | pieceBitboards[black][king] | pieceBitboards[black][queen];
	whitePieces = pieceBitboards[white][pawn] | pieceBitboards[white][rook] | pieceBitboards[white][knight] | pieceBitboards[white][bishop] | pieceBitboards[white][king] | pieceBitboards[white][queen];

	allPieces = whitePieces | blackPieces;
}

void Board::nextMove()
{
	nextColour = switchColour(nextColour);
	kingDangerSquares = 0;
	moveHistory.push_back(zorbistKey);
	update();
}

void Board::loadFromFen(std::string fen)
{
	clearBoard();

	int currentPosInBoard = 56;
	int currentCharPos = 0;
	char currentChar = 'X';

	while (!isspace(currentChar))
	{
		currentChar = fen[currentCharPos];

		if (currentChar == 'p')
		{
			pieceBitboards[black][pawn] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'r')
		{
			pieceBitboards[black][rook] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'n')
		{
			pieceBitboards[black][knight] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'b')
		{
			pieceBitboards[black][bishop] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'k')
		{
			pieceBitboards[black][king] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'q')
		{
			pieceBitboards[black][queen] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'P')
		{
			pieceBitboards[white][pawn] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'R')
		{
			pieceBitboards[white][rook] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'N')
		{
			pieceBitboards[white][knight] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'B')
		{
			pieceBitboards[white][bishop] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'K')
		{
			pieceBitboards[white][king] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == 'Q')
		{
			pieceBitboards[white][queen] |= (uint64_t)1 << currentPosInBoard;
			currentPosInBoard++;
		}
		else if (currentChar == '/')
		{
			currentPosInBoard -= 16;
		}
		else
		{
			currentPosInBoard += (currentChar - '0');
		}
		currentCharPos++;
	}

	if (fen[currentCharPos] == 'w')
	{
		nextColour = white;
	}
	else if (fen[currentCharPos] == 'b')
	{
		nextColour = black;
	}

	currentCharPos += 2;
	currentChar = fen[currentCharPos];

	while (!isspace(currentChar))
	{
		if (currentChar == 'K')
		{
			canWhiteCastleKingSide = true;
		}
		else if (currentChar == 'Q')
		{
			canWhiteCastleQueenSide = true;
		}
		else if (currentChar == 'k')
		{
			canBlackCastleKingSide = true;
		}
		else if (currentChar == 'q')
		{
			canBlackCastleQueenSide = true;
		}
		currentCharPos++;
		currentChar = fen[currentCharPos];
	}


	currentCharPos++;
	currentChar = fen[currentCharPos];
	if (currentChar != '-')
	{
		//En passant target square
		int column = currentChar - 'a';
		int row = fen[currentCharPos + 1] - '1';
		int pos = row * 8 + column;

		enPassantSquare = pos;
	}
	else
	{
		enPassantSquare = -1;
	}

	currentCharPos += 2;

	//Halfmove clock
	//Fullmove clock	

	updateScoreValues();
	generateZorbistKey();
	update();

}

//Outputs the state of the board in the fen format. 
//see this link for more details https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
std::string Board::exportAsFen()
{
	std::string fenString;

	int emptySquaresCounter = 0;

	//Outputs the position of all pieces.
	int counter = 56;
	while (counter >= 0)
	{
		uint64_t currentPieceBitboard = (uint64_t)1 << counter;
		pieceType currentSquarePiece = getPieceTypeInSquare(currentPieceBitboard);

		if (currentSquarePiece == blank)
		{
			emptySquaresCounter++;
		}
		else
		{
			if (emptySquaresCounter > 0)
			{
				fenString += std::to_string(emptySquaresCounter);
				emptySquaresCounter = 0;
			}


			char pieceChar;
			switch (currentSquarePiece)
			{
			case pawn:
				pieceChar = 'p';
				break;
			case knight:
				pieceChar = 'n';
				break;
			case bishop:
				pieceChar = 'b';
				break;
			case rook:
				pieceChar = 'r';
				break;
			case queen:
				pieceChar = 'q';
				break;
			case king:
				pieceChar = 'k';
				break;
			}

			//Converts to uppercase if the piece is white
			if ((whitePieces & currentPieceBitboard) > 0) pieceChar = toupper(pieceChar);

			fenString += pieceChar;
		
		}

		counter++;
		if (counter % 8 == 0)
		{
			if (emptySquaresCounter > 0)
			{
				char emptySquareCounterChar = '0' + emptySquaresCounter;
				fenString += emptySquareCounterChar;
				emptySquaresCounter = 0;
			}
			if(counter != 8) fenString += "/";
			counter -= 16;
		}
	}

	fenString += ' ';

	//Next player to move
	if (nextColour == white) fenString += "w";
	else fenString += "b";

	fenString += ' ';

	//Castling Rights
	if (canWhiteCastleKingSide) fenString += "K";
	if (canWhiteCastleQueenSide) fenString += "Q";
	if (canBlackCastleKingSide) fenString += "k";
	if (canBlackCastleQueenSide) fenString += "q";
	if (!canWhiteCastleKingSide && !canWhiteCastleQueenSide && !canBlackCastleKingSide && !canBlackCastleQueenSide)
		fenString += "-";

	fenString += " ";

	if (enPassantSquare != -1) 
	{
		std::string notation;

		int column = (enPassantSquare % 8);
		int row = 1 + enPassantSquare / 8;

		char rowChar = row + '0';
		char columnChar = column + 'a';
		notation += columnChar;
		notation += rowChar;

		fenString += notation;
	}
	else fenString += "-";

	//Halfmove clock and fullmove number are not currently tracked by this class.
	fenString += " 0 0";

	return fenString;
}

uint64_t Board::getPieceBitboard(colours colour, pieceType piece)
{
	return pieceBitboards[colour][piece];
}

void Board::setBitboard(colours colour, pieceType piece, uint64_t bitboard)
{
	pieceBitboards[colour][piece] = bitboard;
}

void Board::removePiece(uint64_t bitboard)
{
	colours colour;
	if (bitboard & whitePieces)
		colour = white;
	else
		colour = black;

	for (int piece = 0; piece <= 6; piece++)
	{
		if ((pieceBitboards[colour][piece] & bitboard) != 0)
		{
			pieceBitboards[colour][piece] = pieceBitboards[colour][piece] & ~bitboard;
			return;
		}
	}
}

pieceType Board::getPieceTypeInSquare(uint64_t bitboard)
{
	for (int piece = 0; piece <= 6; piece++)
		if (bitboard & (pieceBitboards[black][piece] | pieceBitboards[white][piece])) return (pieceType)piece;
	return blank;
}

bool Board::isPieceAttacked(int piecePos, colours colour)
{
	uint64_t pieceBitboard = (uint64_t)1 << piecePos;

	if (colour == white)
	{
		if ((pieceBitboard << 7) & pieceBitboards[black][pawn] & ~fileH || (pieceBitboard << 9) & pieceBitboards[black][pawn] & ~fileA)
		{
			return true;
		}
	}
	else
	{
		if ((pieceBitboard >> 9) & pieceBitboards[white][pawn] & ~fileH || (pieceBitboard >> 7) & pieceBitboards[white][pawn] & ~fileA)
		{
			return true;
		}
	}

	//KnightMoves
	uint64_t knightMoves = knightMovesArray[piecePos];

	if (colour == white)
	{
		if (knightMoves & pieceBitboards[black][knight])
		{
			return true;
		}
	}
	else
	{
		if (knightMoves & pieceBitboards[white][knight])
		{
			return true;
		}
	}

	uint64_t kingMoves = kingMovesArray[piecePos];

	if (colour == white)
	{
		if (knightMoves & pieceBitboards[black][king])
		{
			return true;
		}
	}
	else
	{
		if (knightMoves & pieceBitboards[white][king])
		{
			return true;
		}
	}
	
	//Rook and half of queen moves
	uint64_t occupancy = magicBitboards::rookMask[piecePos] & allPieces;
	uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[piecePos];
	int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[piecePos];
	uint64_t magicMoves = magicBitboards::magicMovesRook[piecePos][arrayIndex];

	if (colour == white)
	{
		if (magicMoves & (pieceBitboards[black][rook] | pieceBitboards[black][queen]))
		{
			return true;
		}
	}
	else
	{
		if (magicMoves & (pieceBitboards[white][rook] | pieceBitboards[white][queen]))
		{
			return true;
		}
	}

	//Bishop and half of queen moves
	occupancy = magicBitboards::bishopMask[piecePos] & allPieces;
	magicResult = occupancy * magicBitboards::magicNumberBishop[piecePos];
	arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[piecePos];
	magicMoves = magicBitboards::magicMovesBishop[piecePos][arrayIndex];

	if (colour == white)
	{
		if (magicMoves & (pieceBitboards[black][bishop] | pieceBitboards[black][queen]))
		{
			return true;
		}
	}
	else
	{
		if (magicMoves & (pieceBitboards[white][bishop] | pieceBitboards[white][queen]))
		{
			return true;
		}
	}
	return false;
}

void Board::generateZorbistKey()
{
	update();

	uint64_t hash = 0;
	uint64_t pawnHash = 0;

	for (int x = 0; x < 64; x++)
	{
		uint64_t currentPosBitboard = (uint64_t)1 << x;
		if (currentPosBitboard & allPieces) //If their is a piece at the square.
		{
			colours colour;
			if (whitePieces & currentPosBitboard) colour = white;
			else colour = black;

			for (int piece = 0; piece < 6; piece++)
			{
				if ((pieceBitboards[colour][piece] & currentPosBitboard) > 0)
				{
					hash ^= ZorbistKeys::pieceKeys[x][colour * 6 + piece];

					if (piece == pawn)
					{
						pawnHash ^= ZorbistKeys::pieceKeys[x][colour * 6 + piece];
					}
				}
			}
		}
	}
	if (nextColour == black)
		hash ^= ZorbistKeys::blackMoveKey;
	if(canBlackCastleQueenSide)
		hash ^= ZorbistKeys::blackQueenSideCastlingKey;
	if (canBlackCastleKingSide)
		hash ^= ZorbistKeys::blackKingSideCastlingKey;
	if (canWhiteCastleQueenSide)
		hash ^= ZorbistKeys::whiteQueenSideCastlingKey;
	if (canWhiteCastleKingSide)
		hash ^= ZorbistKeys::whiteKingSideCastlingKey;
	if (enPassantSquare != -1)
		hash ^= ZorbistKeys::enPassantKeys[enPassantSquare % 8]; //Adds the hash for the column the of en passant square

	zorbistKey = hash;
	pawnScoreZorbistKey = pawnHash;
}

bool Board::isInCheck()
{
	if (kingDangerSquares == 0)	generateKingDangerSquares();
	return kingDangerSquares & getPieceBitboard(nextColour, king);
}

uint64_t Board::getKingDangerSquares()
{
	if (kingDangerSquares == 0)	generateKingDangerSquares();
	return kingDangerSquares;
}

void Board::generateKingDangerSquares()
{
	const uint64_t allPiecesExceptKing = allPieces & ~getPieceBitboard(nextColour, king);
	const colours oppositeColour = switchColour(nextColour);

	uint64_t attackSet = 0;
	uint64_t currentPos;

	uint64_t pawnBitboard = getPieceBitboard(oppositeColour, pawn);
	if (oppositeColour == white)
	{
		while (pawnBitboard)
		{
			currentPos = pop(pawnBitboard);
			attackSet |= currentPos << 7 & ~fileH | currentPos << 9 & ~fileA;
		}
	}
	else
	{
		while (pawnBitboard)
		{
			currentPos = pop(pawnBitboard);
			attackSet |= (currentPos >> 9) & ~fileH | (currentPos >> 7) & ~fileA;
		}
	}

	uint64_t knightBitboard = getPieceBitboard(oppositeColour, knight);
	while (knightBitboard)
	{
		currentPos = pop(knightBitboard);
		attackSet |= knightMovesArray[bitScanForward(currentPos)];
	}

	uint64_t kingBitboard = getPieceBitboard(oppositeColour, king);
	while (kingBitboard)
	{
		currentPos = pop(kingBitboard);
		attackSet |= kingMovesArray[bitScanForward(currentPos)];
	}

	uint64_t rookBitboard = getPieceBitboard(oppositeColour, rook) | getPieceBitboard(oppositeColour, queen);
	while (rookBitboard)
	{
		currentPos = pop(rookBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::rookMask[piecePos] & allPiecesExceptKing;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberRook[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftRook[piecePos];
		attackSet |= magicBitboards::magicMovesRook[piecePos][arrayIndex];
	}

	uint64_t bishopBitboard = getPieceBitboard(oppositeColour, bishop) | getPieceBitboard(oppositeColour, queen);
	while (bishopBitboard)
	{
		currentPos = pop(bishopBitboard);
		int piecePos = bitScanForward(currentPos);

		uint64_t occupancy = magicBitboards::bishopMask[piecePos] & allPiecesExceptKing;
		uint64_t magicResult = occupancy * magicBitboards::magicNumberBishop[piecePos];
		int arrayIndex = magicResult >> magicBitboards::magicNumberShiftBishop[piecePos];
		attackSet |= magicBitboards::magicMovesBishop[piecePos][arrayIndex];
	}

	kingDangerSquares = attackSet;
}

int Board::getMaterialScore(colours colour)
{
	if (colour == white) return whitePawnScore + whiteMaterialScore - blackPawnScore - blackMaterialScore;
	else return blackPawnScore + blackMaterialScore - whitePawnScore - whiteMaterialScore;
}

int Board::getOnlyMaterialScore(colours colour)
{
	if (colour == white) return whiteMaterialScore;
	else return blackMaterialScore;
}

int Board::getPositionalScore(colours colour)
{
	if (colour == white) return whitePositionalScore - blackPositionalScore;
	else return whitePositionalScore - blackPositionalScore;
}

int Board::getMidGameKingPositionalScore(colours colour)
{
	if (colour == white) return whiteMidGameKingPositionalScore;
	else return blackMidGameKingPositionalScore;
}

int Board::getLateGameKingPositionalScore(colours colour)
{
	if (colour == white) return whiteLateGameKingPositionalScore;
	else return blackLateGameKingPositionalScore;
}

void Board::updateScoreValues()
{
	whitePawnScore = bitSum(getPieceBitboard(white, pawn)) * 100;

	whiteMaterialScore = 0;
	whiteMaterialScore += bitSum(getPieceBitboard(white, knight)) * 300;
	whiteMaterialScore += bitSum(getPieceBitboard(white, bishop)) * 300;
	whiteMaterialScore += bitSum(getPieceBitboard(white, rook)) * 500;
	whiteMaterialScore += bitSum(getPieceBitboard(white, queen)) * 900;

	blackPawnScore = bitSum(getPieceBitboard(black, pawn)) * 100;

	blackMaterialScore = 0;
	blackMaterialScore += bitSum(getPieceBitboard(black, knight)) * 300;
	blackMaterialScore += bitSum(getPieceBitboard(black, bishop)) * 300;
	blackMaterialScore += bitSum(getPieceBitboard(black, rook)) * 500;
	blackMaterialScore += bitSum(getPieceBitboard(black, queen)) * 900;

	whitePositionalScore = 0;
	whitePositionalScore += pieceSquareData::pawnSquare.calcScore(getPieceBitboard(white, pawn), white);
	whitePositionalScore += pieceSquareData::knightSquare.calcScore(getPieceBitboard(white, knight), white);
	whitePositionalScore += pieceSquareData::bishopSquare.calcScore(getPieceBitboard(white, bishop), white);
	whiteMidGameKingPositionalScore = pieceSquareData::midGameKingSquare.calcScore(getPieceBitboard(white, king), white);
	whiteLateGameKingPositionalScore = pieceSquareData::lateGameKingSquare.calcScore(getPieceBitboard(white, king), white);
	
	blackPositionalScore = 0;
	blackPositionalScore += pieceSquareData::pawnSquare.calcScore(getPieceBitboard(black, pawn), black);
	blackPositionalScore += pieceSquareData::knightSquare.calcScore(getPieceBitboard(black, knight), black);
	blackPositionalScore += pieceSquareData::bishopSquare.calcScore(getPieceBitboard(black, bishop), black);
	blackMidGameKingPositionalScore = pieceSquareData::midGameKingSquare.calcScore(getPieceBitboard(black, king), black);
	blackLateGameKingPositionalScore = pieceSquareData::lateGameKingSquare.calcScore(getPieceBitboard(black, king), black);
}

void Board::addMaterialScore(colours colour, pieceType piece)
{
	const int materialValues[6] = { 100,300,300,500,900,0 };
	if (colour == white)
	{
		if (piece == pawn)
		{
			whitePawnScore += materialValues[pawn];
		}
		else
		{
			whiteMaterialScore += materialValues[piece];
		}

	}
	else
	{
		if (piece == pawn)
		{
			blackPawnScore += materialValues[pawn];
		}
		else
		{
			blackMaterialScore += materialValues[piece];
		}
	}
}

void Board::removeMaterialScore(colours colour, pieceType piece)
{
	const int materialValues[6] = { 100,300,300,500,900,0 };
	if (colour == white)
	{
		if (piece == pawn)
		{
			whitePawnScore -= materialValues[pawn];
		}
		else
		{
			whiteMaterialScore -= materialValues[piece];
		}
	}
	else
	{
		if (piece == pawn)
		{
			blackPawnScore -= materialValues[pawn];
		}
		else
		{
			blackMaterialScore -= materialValues[piece];
		}
	}
}

void Board::addPositionalScore(colours colour, pieceType piece, int piecePos)
{
	if (colour == white)
	{
		if (piece == pawn)
		{
			whitePositionalScore += pieceSquareData::pawnSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == knight)
		{
			whitePositionalScore += pieceSquareData::knightSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == bishop)
		{
			whitePositionalScore += pieceSquareData::bishopSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == king)
		{
			whiteMidGameKingPositionalScore += pieceSquareData::midGameKingSquare.getScoreFromPos(piecePos, white);
			whiteLateGameKingPositionalScore += pieceSquareData::lateGameKingSquare.getScoreFromPos(piecePos, white);
		}
	}
	else
	{
		if (piece == pawn)
		{
			blackPositionalScore += pieceSquareData::pawnSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == knight)
		{
			blackPositionalScore += pieceSquareData::knightSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == bishop)
		{
			blackPositionalScore += pieceSquareData::bishopSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == king)
		{
			blackMidGameKingPositionalScore += pieceSquareData::midGameKingSquare.getScoreFromPos(piecePos, black);
			blackLateGameKingPositionalScore += pieceSquareData::lateGameKingSquare.getScoreFromPos(piecePos, black);
		}
	}
}

void Board::removePositionalScore(colours colour, pieceType piece, int piecePos)
{
	if (colour == white)
	{
		if (piece == pawn)
		{
			whitePositionalScore -= pieceSquareData::pawnSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == knight)
		{
			whitePositionalScore -= pieceSquareData::knightSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == bishop)
		{
			whitePositionalScore -= pieceSquareData::bishopSquare.getScoreFromPos(piecePos, white);
		}
		else if (piece == king)
		{
			whiteMidGameKingPositionalScore -= pieceSquareData::midGameKingSquare.getScoreFromPos(piecePos, white);
			whiteLateGameKingPositionalScore -= pieceSquareData::lateGameKingSquare.getScoreFromPos(piecePos, white);
		}
	}
	else
	{
		if (piece == pawn)
		{
			blackPositionalScore -= pieceSquareData::pawnSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == knight)
		{
			blackPositionalScore -= pieceSquareData::knightSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == bishop)
		{
			blackPositionalScore -= pieceSquareData::bishopSquare.getScoreFromPos(piecePos, black);
		}
		else if (piece == king)
		{
			blackMidGameKingPositionalScore -= pieceSquareData::midGameKingSquare.getScoreFromPos(piecePos, black);
			blackLateGameKingPositionalScore -= pieceSquareData::lateGameKingSquare.getScoreFromPos(piecePos, black);
		}
	}
}


