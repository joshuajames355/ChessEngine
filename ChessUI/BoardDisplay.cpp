#include "BoardDisplay.h"



BoardDisplay::BoardDisplay(QWidget *parent) : QGraphicsView(parent)
{
	movingPiece = nullptr;
	isPlayersTurn = true;

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	setScene(&graphicsScene);
	graphicsScene.setBackgroundBrush(Qt::green);

	QPixmap blackSquarePixmap;
	blackSquarePixmap.load("blackSquare.png");
	QPixmap whiteSquarePixmap;
	whiteSquarePixmap.load("whiteSquare.png");

	positionLabelSize = 50;
	squareSize = 100;
	pieceSize = 75;

	setMinimumSize(squareSize * 8 + positionLabelSize * 2, squareSize * 8 + positionLabelSize * 2);

	resize(100 * 8, 100 * 8);

	//Setup move generation
	magicBitboards temp = magicBitboards();
	temp.setupMagicBitboards();
	setupBitboardUtils();
	setupMoveGen();

	//Adds the chess squares.
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
			boardSquares[x][y].setOffset(positionLabelSize + squareSize * x, positionLabelSize + squareSize * (7 - y));

			boardSquares[x][y].setScale(squareSize / boardSquares[x][y].boundingRect().width());

			boardSquares[x][y].setZValue(-1);

			graphicsScene.addItem(&boardSquares[x][y]);
		}
	}

	QFont font = QFont("Times", 15, QFont::Bold);

	//Adds the position Labels
	for (int x = 0; x < 8; x++)
	{
		char equivalentLetter = 'a' + x;
		char numericalLetter = '8' - x;

		//Top
		QGraphicsTextItem*  newLabel = new QGraphicsTextItem;
		newLabel->setFont(font);
		newLabel->setPos(1.5 * positionLabelSize + squareSize * x, 0.5 * positionLabelSize);
		newLabel->setPlainText(QString(equivalentLetter));
		graphicsScene.addItem(newLabel);
		positionLabelsTop[x] = newLabel;

		//Bottom
		newLabel = new QGraphicsTextItem;
		newLabel->setFont(font);
		newLabel->setPos(1.5 * positionLabelSize + squareSize * x, 1.5 * positionLabelSize + 8 * squareSize);
		newLabel->setPlainText(QString(equivalentLetter));
		graphicsScene.addItem(newLabel);
		positionLabelsBottom[x] = newLabel;

		//Left
		newLabel = new QGraphicsTextItem;
		newLabel->setFont(font);
		newLabel->setPos(0.5 * positionLabelSize, 1.5 * positionLabelSize + x * squareSize);
		newLabel->setPlainText(QString(numericalLetter));
		graphicsScene.addItem(newLabel);
		positionLabelsLeft[x] = newLabel;

		//Right
		newLabel = new QGraphicsTextItem;
		newLabel->setFont(font);
		newLabel->setPos(1.5 * positionLabelSize + squareSize * 8, 1.5 * positionLabelSize + x * squareSize);
		newLabel->setPlainText(QString(numericalLetter));
		graphicsScene.addItem(newLabel);
		positionLabelsRight[x] = newLabel;
	}

	chessBoard.defaults();
	loadChessPiecePixmaps();
	updateChessPieces();
}

void BoardDisplay::mousePressEvent(QMouseEvent * event)
{
	if (!isPlayersTurn) return;
	for (int x = 0; x < chessPieces.size(); x++)
	{
		if (chessPieces[x]->isUnderMouse() && chessPieces[x]->getColour() == chessBoard.nextColour)
		{
			isPieceBeingDragged = true;
			movingPiece = chessPieces[x];
			originalOffset = movingPiece->offset();
			movingPiece->setZValue(1);
		}
	}
}

void BoardDisplay::mouseMoveEvent(QMouseEvent * event)
{
	if (isPieceBeingDragged && movingPiece != nullptr)
	{
		movingPiece->setOffset(positionLabelSize + event->x() - pieceSize, positionLabelSize + event->y() - pieceSize);
	}
}

void BoardDisplay::mouseReleaseEvent(QMouseEvent * event)
{
	Move move;
	if (isPieceBeingDragged && movingPiece != nullptr)
	{
		int newPos = -1;
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				if (boardSquares[x][y].isUnderMouse())
				{
					newPos = y * 8 + x;
				}
			}
		}

		uint64_t moveToBitboard = (uint64_t)1 << newPos;
		if (newPos != -1)
		{
			//Todo , pawn promotion
			if ((chessBoard.allPieces & moveToBitboard) == 0 && std::abs(newPos - movingPiece->getPiecePosition()) == 16 && movingPiece->getPieceType() == pawn)
				move = Move(movingPiece->getPiecePosition(), newPos, pawnDoubleMove, pawn, &chessBoard);
			else if((chessBoard.allPieces & moveToBitboard) == 0 && newPos - movingPiece->getPiecePosition() == 2 && movingPiece->getPieceType() == king)
				move = Move(movingPiece->getPiecePosition(), newPos, kingSideCastling, king, &chessBoard);
			else if ((chessBoard.allPieces & moveToBitboard) == 0 && newPos - movingPiece->getPiecePosition() == -2 && movingPiece->getPieceType() == king)
				move = Move(movingPiece->getPiecePosition(), newPos, queenSideCastling, king, &chessBoard);

			//Pawn Promotion Moves
			else if (movingPiece->getPieceType() == pawn && (moveToBitboard & (rank1 | rank8)) > 0)
			{
				MoveType type = quietMove;
				while (type == quietMove)
				{
					PawnPromotionDialog pawnDialog;
					pawnDialog.setupDialogBox(&piecePixmaps, chessBoard.nextColour);
					pawnDialog.exec();
					type = pawnDialog.getCurrentType();
				}
				move = Move(movingPiece->getPiecePosition(), newPos, type, movingPiece->getPieceType(), &chessBoard);
			}

			else if ((chessBoard.allPieces & moveToBitboard) == 0)
				move = Move(movingPiece->getPiecePosition(), newPos, quietMove, movingPiece->getPieceType(), &chessBoard);
			else if ((chessBoard.allPieces & moveToBitboard) != 0)
				move = Move(movingPiece->getPiecePosition(), newPos, capture, movingPiece->getPieceType(), &chessBoard);
		}
	}

	bool isValidMove = false;

	for (int x = 0; x < moveListSize; x++)
	{
		if (moveList[x] == move)
			isValidMove = true;
	}


	//If invalid move, move back to original position
	if (isPieceBeingDragged && movingPiece != nullptr && !isValidMove)
	{
		movingPiece->setOffset(originalOffset);
		movingPiece->setZValue(0);
	}
	else if(isPieceBeingDragged && movingPiece != nullptr)
	{
		applyMove(move);
	}

	isPieceBeingDragged = false;
	movingPiece = nullptr;

}

void BoardDisplay::setBoard(Board newBoard)
{
	chessBoard = newBoard;
	updateChessPieces();
}

Board BoardDisplay::getBoard()
{
	return chessBoard;
}

void BoardDisplay::applyMove(Move newMove)
{
	playedMoves.push_back(newMove);
	newMove.applyMove(&chessBoard);
	updateChessPieces();
	emit newTurn();
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

			ChessPiece* currentChessPiece = new ChessPiece();

			currentChessPiece->setPixmap(piecePixmaps[currentType][currentColour]);

			qreal widthOffset = positionLabelSize + squareSize * (counter % 8) + squareSize / 2 - currentChessPiece->boundingRect().width() / 2;
			qreal heightOffset = positionLabelSize + squareSize * (7 - (counter / 8)) + squareSize / 2 - currentChessPiece->boundingRect().height() / 2;
			currentChessPiece->setOffset(widthOffset, heightOffset);

			currentChessPiece->setPiecePosition(counter);
			currentChessPiece->setColour(currentColour);
			currentChessPiece->setPieceType(currentType);

			graphicsScene.addItem(currentChessPiece);
			chessPieces.push_back(currentChessPiece);
		}
	}

	moveListSize = searchForMoves(&chessBoard, &moveList);
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
