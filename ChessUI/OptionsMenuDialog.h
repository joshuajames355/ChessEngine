#pragma once
#include "qdialog.h"
#include <qlayout.h>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <qpushbutton.h>

#include "piece.h"

struct Options
{
	Options();

	bool isAi;
	colours aiColour;
};

class OptionsMenuDialog :
	public QDialog
{
	Q_OBJECT

public:
	OptionsMenuDialog();
	~OptionsMenuDialog();

	void setupDialogBox(Options* currentOptions);
	Options getOptions();

private:
	QCheckBox* isAiCheckbox;
	QHBoxLayout* aiColourHorizontalLayout;
	QLabel* aiColourLabel;
	QComboBox* aiColourComboBox;
	QVBoxLayout* verticalLayout;
	QPushButton* okButton;

private slots:
	void doneButtonCallback();

};

