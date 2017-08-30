#pragma once
#include <QProcess>
#include "move.h"
#include "utils.h"
#include "EngineOutputDialog.h"

enum AIStatus{stopped, ready, waitingForBestmove, waitingForReady};

class AIManager : public QObject
{
	Q_OBJECT

public:
	AIManager();
	~AIManager();
	void startAI();
	void showEngineOutputDialog();
	void findMove(Board board);

signals:
	void newMove(Move newMove);

private:
	AIStatus status;
	QProcess* aiProcess;
	EngineOutputDialog engineOutputDialog;
	void sendCommand(QString command);
	Board lastBoardState;

private slots:
	void processInputCallback();
};

