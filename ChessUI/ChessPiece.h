#pragma once
#include <QGraphicsPixmapItem>

#include "piece.h"

class ChessPiece :
	public QGraphicsPixmapItem
{
public:
	ChessPiece(QGraphicsItem * parent);
	ChessPiece();
	~ChessPiece();

	inline void setPiecePosition(int newPos) { pos = newPos; };
	inline int getPiecePosition() { return pos; };
	inline void setPieceType(pieceType newPieceType) { piece = newPieceType; };
	inline pieceType getPieceType() { return piece; };
	inline void setColour(colours newColour) { colour = newColour; };
	inline colours getColour() { return colour; };

private:
	int pos;
	pieceType piece;
	colours colour;
};

