#include "GameManager.h"



GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
	boardDisplay = new BoardDisplay(this);

	connect(boardDisplay, SIGNAL(newTurn()), this, SLOT(newTurn()));
	connect(&aiManager, SIGNAL(newMove(Move)), this, SLOT(aiNewMove(Move)));
}

GameManager::~GameManager()
{
	delete boardDisplay;
}

void GameManager::loadFromFile()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open file"), "", tr("Chess Files (*.pgn *.fen)"));

	std::ifstream file;
	file.open(filename.toStdString());

	std::stringstream strStream;
	strStream << file.rdbuf();

	std::string fileContents = strStream.str();

	//PGN file type
	if (fileContents[0] == '[')
	{
		//TODO
	}
	else
	{
		Board newBoard;
		newBoard.loadFromFen(fileContents);
		boardDisplay->setBoard(newBoard);
	}
	
}

void GameManager::saveToFile()
{
	std::string fenData = boardDisplay->getBoard().exportAsFen();
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Open file"), "", tr("Chess Files (*.pgn *.fen)"));

	std::ofstream file(filename.toStdString());
	file << fenData;
	file.close();
}

void GameManager::displayOptionsMenu()
{
	OptionsMenuDialog dialog;
	dialog.setupDialogBox(&currentOptions);
	dialog.exec();
	currentOptions = dialog.getOptions();
	newTurn();
}

void GameManager::newTurn()
{
	if (!currentOptions.isAi || (currentOptions.isAi & currentOptions.aiColour != boardDisplay->getBoard().nextColour))
	{
		boardDisplay->setIsPlayersTurn(true);
	}
	else
	{
		boardDisplay->setIsPlayersTurn(false);
	}

	if (currentOptions.isAi && currentOptions.aiColour == boardDisplay->getBoard().nextColour)
	{
		aiManager.startAI();
		aiManager.findMove(boardDisplay->getBoard());
	}
}

void GameManager::displayEngineOutputMenu()
{
	aiManager.showEngineOutputDialog();
}

void GameManager::flipBoard()
{
	boardDisplay->flipBoard();
}

void GameManager::aiNewMove(Move newMove)
{
	boardDisplay->applyMove(newMove);
}
