#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMouseEvent>

#include <algorithm>
#include <vector>
#include <array>
#include <stdint.h>

#include "magicBitboards.h"
#include "ChessPiece.h"
#include "board.h"
#include "piece.h"
#include "move.h"
#include "moveGeneration.h"
#include "PawnPromotionDialog.h"

class BoardDisplay :
	public QGraphicsView
{

	Q_OBJECT

public:
	BoardDisplay(QWidget *parent);
	~BoardDisplay();

private:
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
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	QPointF originalOffset;

	Board chessBoard;
	std::vector<Move> playedMoves;

	int moveListSize;
	std::array<Move, 150> moveList;
};

