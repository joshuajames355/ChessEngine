#include "BoardDisplay.h"



BoardDisplay::BoardDisplay(QWidget *parent) : QGraphicsView(parent)
{
	setScene(&graphicsScene);
	graphicsScene.setBackgroundBrush(Qt::green);

	QPixmap blackSquarePixmap;
	blackSquarePixmap.load("blackSquare.png");
	QPixmap whiteSquarePixmap;
	whiteSquarePixmap.load("whiteSquare.png");

	squareSize = 100;
	pieceSize = 75;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			//If it is a black square on the chess board
			if (y % 2 == 0 & x % 2 == 0 || y % 2 == 1 & x % 2 == 1)
				boardSquares[x][y].setPixmap(blackSquarePixmap);
			else
				boardSquares[x][y].setPixmap(whiteSquarePixmap);

			//Sets the offsets for the squares , flipping vertically as all internal chess representations start from 
			//bottom left hand corner, and qt starts from the top left.
			boardSquares[x][y].setOffset(squareSize * x, squareSize * (8 - y));

			boardSquares[x][y].setScale(squareSize / boardSquares[x][y].boundingRect().width());
			
			graphicsScene.addItem(&boardSquares[x][y]);
		}
	}

	chessBoard.defaults();
	loadChessPiecePixmaps();
	updateChessPieces();
}


BoardDisplay::~BoardDisplay()
{
}

void BoardDisplay::updateChessPieces()
{

	//Deletes pieces
	for (int x = 0; x < chessPieces.size(); x++)
	{
		graphicsScene.removeItem(chessPieces[x]);
		delete chessPieces[x];
	}
	chessPieces.clear();

	for (int counter = 0; counter < 64; counter++)
	{
		uint64_t currentPosBitboard = (uint64_t)1 << counter;
		pieceType currentType = chessBoard.getPieceTypeInSquare(currentPosBitboard);
		
		if (currentType != blank)
		{
			colours currentColour;
			if (chessBoard.findBitboard(white, currentType) & currentPosBitboard)
				currentColour = white;
			else
				currentColour = black;

			QGraphicsPixmapItem* currentChessPiece = new QGraphicsPixmapItem();

			currentChessPiece->setPixmap(piecePixmaps[currentType][currentColour]);

			qreal widthOffset = squareSize * (counter % 8) + squareSize / 2 - currentChessPiece->boundingRect().width() / 2;
			qreal heightOffset = squareSize * (8 - (counter / 8)) + squareSize / 2 - currentChessPiece->boundingRect().height() / 2;
			currentChessPiece->setOffset(widthOffset, heightOffset);

			graphicsScene.addItem(currentChessPiece);
			chessPieces.push_back(currentChessPiece);
		}
	}
}

void BoardDisplay::loadChessPiecePixmaps()
{
	piecePixmaps[pawn][white].load("whitePawn.png");
	piecePixmaps[knight][white].load("whiteKnight.png");
	piecePixmaps[bishop][white].load("whiteBishop.png");
	piecePixmaps[rook][white].load("whiteRook.png");
	piecePixmaps[queen][white].load("whiteQueen.png");
	piecePixmaps[king][white].load("whiteKing.png");

	piecePixmaps[pawn][black].load("blackPawn.png");
	piecePixmaps[knight][black].load("blackKnight.png");
	piecePixmaps[bishop][black].load("blackBishop.png");
	piecePixmaps[rook][black].load("blackRook.png");
	piecePixmaps[queen][black].load("blackQueen.png");
	piecePixmaps[king][black].load("blackKing.png");

	//Scales all pieces to pieceSize (assuming they have the same height and width
	for (int x = 0; x < 6; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			piecePixmaps[x][y] = piecePixmaps[x][y].scaledToHeight(pieceSize);
		}
	}
}
