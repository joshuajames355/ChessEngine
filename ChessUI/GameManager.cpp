#include "GameManager.h"



GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
	boardDisplay = new BoardDisplay(this);
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
}
