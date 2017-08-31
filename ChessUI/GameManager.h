#pragma once
#include "qwidget.h"

#include "BoardDisplay.h"
#include "board.h"
#include "OptionsMenuDialog.h"
#include "AIManager.h"


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
	void displayEngineOutputMenu();
	void flipBoard();

private slots:
	void newTurn();
	void aiNewMove(Move newMove);

private:
	BoardDisplay* boardDisplay;
	Options currentOptions;
	AIManager aiManager;
};

