#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMouseEvent>
#include <QFileDialog>

#include <algorithm>
#include <vector>
#include <array>
#include <stdint.h>
#include <fstream>
#include <sstream>

#include "magicBitboards.h"
#include "ChessPiece.h"
#include "board.h"
#include "piece.h"
#include "move.h"
#include "moveGeneration.h"
#include "PawnPromotionDialog.h"
#include "OptionsMenuDialog.h"

class BoardDisplay :
	public QGraphicsView
{
	Q_OBJECT

public:
	BoardDisplay(QWidget *parent);
	void setBoard(Board newBoard);
	Board getBoard();
	void setIsPlayersTurn(bool newIsPlayersTurn) { isPlayersTurn = newIsPlayersTurn; };
	void applyMove(Move newMove);
	void flipBoard();
	void newGame();

signals:
	void newTurn();

private:
	QPixmap blackSquarePixmap;
	QPixmap whiteSquarePixmap;
	qreal positionLabelSize;
	qreal squareSize;
	QGraphicsPixmapItem boardSquares[8][8];
	QGraphicsScene graphicsScene;

	qreal pieceSize;
	std::vector<ChessPiece*> chessPieces;
	void updateChessPieces();

	std::array<std::array<QPixmap,2>, 6> piecePixmaps;
	void loadChessPiecePixmaps();

	bool isPieceBeingDragged;
	ChessPiece* movingPiece;
	void addPositionLabels();
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	QPointF originalOffset;

	Board chessBoard;
	std::vector<Move> playedMoves;

	int moveListSize;
	std::array<Move, 150> moveList;

	std::array<QGraphicsTextItem*, 8> positionLabelsTop;
	std::array<QGraphicsTextItem*, 8> positionLabelsBottom;
	std::array<QGraphicsTextItem*, 8> positionLabelsLeft;
	std::array<QGraphicsTextItem*, 8> positionLabelsRight;

	bool isPlayersTurn;
	bool isBoardFlipped;


protected:
	virtual void wheelEvent(QWheelEvent * event) {}; //Overrides the scroll event to disable zooming the graphicsView.
};

