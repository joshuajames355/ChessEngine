#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_chessui.h"

class ChessUI : public QMainWindow
{
	Q_OBJECT

public:
	ChessUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::ChessUIClass ui;
};
