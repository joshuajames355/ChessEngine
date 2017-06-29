#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include <vector>
#include <stdint.h>

#include "board.h"
#include "piece.h"

class BoardDisplay :
	public QGraphicsView
{
public:
	BoardDisplay(QWidget *parent);
	~BoardDisplay();

private:
	qreal squareSize;
	QGraphicsPixmapItem boardSquares[8][8];
	QGraphicsScene graphicsScene;

	qreal pieceSize;
	std::vector<QGraphicsPixmapItem*> chessPieces;
	void updateChessPieces();

	QPixmap piecePixmaps[6][2];
	void loadChessPiecePixmaps();

	Board chessBoard;
};

