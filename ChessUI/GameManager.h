#pragma once
#include "qwidget.h"

#include "BoardDisplay.h"
#include "board.h"
#include "OptionsMenuDialog.h"


class GameManager :
	public QWidget
{
	Q_OBJECT

public:
	GameManager(QWidget *parent);
	~GameManager();

public slots:
	void loadFromFile();
	void saveToFile();
	void displayOptionsMenu();

private:
	BoardDisplay* boardDisplay;
	Options currentOptions;
};

