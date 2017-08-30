#pragma once
#include "qdialog.h"
#include <QTextBrowser>
#include <QPushButton>
#include <qlayout.h>

class EngineOutputDialog :
	public QDialog
{
	Q_OBJECT
public:
	EngineOutputDialog();
	~EngineOutputDialog();
	void setupDialogBox();
	void addNewLine(QString line);

private:
	QPushButton* okButton;
	QTextEdit* engineDisplay;
	QVBoxLayout* verticalLayout;
	QHBoxLayout* lowerBar;
	QPushButton* clearButton;

private slots:
	void okButtonPressedCallback();
};

