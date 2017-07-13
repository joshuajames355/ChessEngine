#include "PawnPromotionDialog.h"



PawnPromotionDialog::PawnPromotionDialog(QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
	currentType = quietMove;
}

PawnPromotionDialog::~PawnPromotionDialog()
{
	delete rookPromotionButton;
	delete bishopPromotionButton;
	delete queenPromotionButton;
	delete knightPromotionButton;
	delete horizontalLayout;
}

void PawnPromotionDialog::setupDialogBox(std::array<std::array<QPixmap, 2>, 6>* piecePixmaps, colours currentColour)
{
	horizontalLayout = new QHBoxLayout(this);

	if (currentColour == black)
	{
		rookPromotionButton = new QPushButton(QIcon((*piecePixmaps)[rook][black]), "Rook", this);
		bishopPromotionButton = new QPushButton(QIcon((*piecePixmaps)[bishop][black]), "Bishop", this);
		queenPromotionButton = new QPushButton(QIcon((*piecePixmaps)[queen][black]), "Queen", this);
		knightPromotionButton = new QPushButton(QIcon((*piecePixmaps)[knight][black]), "Knight", this);
	}
	else
	{
		rookPromotionButton = new QPushButton(QIcon((*piecePixmaps)[rook][white]), "Rook", this);
		bishopPromotionButton = new QPushButton(QIcon((*piecePixmaps)[bishop][white]), "Bishop", this);
		queenPromotionButton = new QPushButton(QIcon((*piecePixmaps)[queen][white]), "Queen", this);
		knightPromotionButton = new QPushButton(QIcon((*piecePixmaps)[knight][white]), "Knight", this);
	}

	connect(rookPromotionButton, SIGNAL(clicked()), this, SLOT(rookPromotionButtonCallback()));
	connect(bishopPromotionButton, SIGNAL(clicked()), this, SLOT(bishopPromotionButtonCallback()));
	connect(queenPromotionButton, SIGNAL(clicked()), this, SLOT(queenPromotionButtonCallback()));
	connect(knightPromotionButton, SIGNAL(clicked()), this, SLOT(knightPromotionButtonCallback()));

	horizontalLayout->addWidget(queenPromotionButton);
	horizontalLayout->addWidget(rookPromotionButton);
	horizontalLayout->addWidget(bishopPromotionButton);
	horizontalLayout->addWidget(knightPromotionButton);
}

void PawnPromotionDialog::rookPromotionButtonCallback()
{
	currentType = rookPromotion;
	emit accept();
}
void PawnPromotionDialog::queenPromotionButtonCallback()
{
	currentType = queenPromotion;
	emit accept();
}
void PawnPromotionDialog::knightPromotionButtonCallback()
{
	currentType = knightPromotion;
	emit accept();
}
void PawnPromotionDialog::bishopPromotionButtonCallback()
{
	currentType = bishopPromotion;
	emit accept();
}
