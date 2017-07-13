#pragma once
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlayout.h>

#include <array>
#include "piece.h"
#include "move.h"

class PawnPromotionDialog :
	public QDialog
{
	Q_OBJECT

public:
	PawnPromotionDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~PawnPromotionDialog();

	void setupDialogBox(std::array<std::array<QPixmap, 2>, 6>* piecePixmaps, colours currentColour);

	inline MoveType getCurrentType() { return currentType; };

private:
	QPushButton* rookPromotionButton;
	QPushButton* bishopPromotionButton;
	QPushButton* queenPromotionButton;
	QPushButton* knightPromotionButton;
	QHBoxLayout* horizontalLayout;

	MoveType currentType;

private slots:
	void rookPromotionButtonCallback();
	void bishopPromotionButtonCallback();
	void queenPromotionButtonCallback();
	void knightPromotionButtonCallback();

};

