#include "EngineOutputDialog.h"

EngineOutputDialog::EngineOutputDialog()
{
	setModal(false);
}


EngineOutputDialog::~EngineOutputDialog()
{
	delete okButton;
	delete engineDisplay;
	delete verticalLayout;
	delete lowerBar;
	delete clearButton;
}

void EngineOutputDialog::setupDialogBox()
{
	verticalLayout = new QVBoxLayout(this);
	setLayout(verticalLayout);

	engineDisplay = new QTextEdit(this);
	engineDisplay->setReadOnly(true);
	engineDisplay->setMinimumSize(300, 400);
	verticalLayout->addWidget(engineDisplay);

	lowerBar = new QHBoxLayout(this);
	verticalLayout->addLayout(lowerBar);

	clearButton = new QPushButton("clear", this);
	lowerBar->addWidget(clearButton);

	okButton = new QPushButton("ok", this);
	lowerBar->addWidget(okButton);

	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPressedCallback()));
	connect(clearButton, SIGNAL(clicked()), engineDisplay, SLOT(clear()));
}

void EngineOutputDialog::okButtonPressedCallback()
{
	emit accept();
}

void EngineOutputDialog::addNewLine(QString line) 
{
	engineDisplay->append(line);
}
