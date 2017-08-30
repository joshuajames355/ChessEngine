#include "AIManager.h"



AIManager::AIManager()
{
	status = stopped;
	aiProcess = new QProcess(this);
	connect(aiProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processInputCallback()));
	engineOutputDialog.setupDialogBox();
}

AIManager::~AIManager()
{
	aiProcess->terminate();
	delete aiProcess;
}

void AIManager::startAI()
{
	if (status == stopped)
	{
		aiProcess->start("ChessEngine.exe");
		aiProcess->setReadChannel(QProcess::StandardOutput);
		sendCommand("isready");
		status = waitingForReady;
	}
}

void AIManager::showEngineOutputDialog()
{
	engineOutputDialog.show();
}

void AIManager::findMove(Board board)
{
	QString positionCommand = "position fen ";
	positionCommand += QString::fromStdString(board.exportAsFen());
	sendCommand(positionCommand);

	sendCommand("go");

	status = waitingForBestmove;
	lastBoardState = board;

}

void AIManager::sendCommand(QString command)
{
	std::string commandWithNewline = command.toStdString() + "\n";
	aiProcess->write(commandWithNewline.c_str());
	engineOutputDialog.addNewLine(command);
}

void AIManager::processInputCallback()
{
	while (aiProcess->canReadLine())
	{
		QString input = aiProcess->readLine().simplified();
		engineOutputDialog.addNewLine(input);
		if (status == waitingForReady)
		{
			if (input.startsWith("readyok"))
			{
				status = ready;
			}
		}
		else if (status == waitingForBestmove)
		{
			if (input.startsWith("bestmove"))
			{
				emit newMove(moveFromNotation(input.remove(0,9).toStdString(), &lastBoardState));
				status = ready;
			}
		}
	}
}