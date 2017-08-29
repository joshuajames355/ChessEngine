#include "OptionsMenuDialog.h"



OptionsMenuDialog::OptionsMenuDialog()
{
}


OptionsMenuDialog::~OptionsMenuDialog()
{
	delete isAiCheckbox;
	delete aiColourHorizontalLayout;
	delete aiColourLabel;
	delete aiColourComboBox;
	delete verticalLayout;
	delete okButton;
}

void OptionsMenuDialog::setupDialogBox(Options * currentOptions)
{
	verticalLayout = new QVBoxLayout(this);

	isAiCheckbox = new QCheckBox("Play against AI?", this);
	isAiCheckbox->setChecked(currentOptions->isAi);
	isAiCheckbox->setLayoutDirection(Qt::RightToLeft);
	verticalLayout->addWidget(isAiCheckbox);

	aiColourHorizontalLayout = new QHBoxLayout(this);
	verticalLayout->addLayout(aiColourHorizontalLayout);

	aiColourLabel = new QLabel("Colour of AI?", this);
	aiColourHorizontalLayout->addWidget(aiColourLabel);

	aiColourComboBox = new QComboBox(this);
	aiColourComboBox->addItem("White");
	aiColourComboBox->addItem("Black");
	if (currentOptions->aiColour == white) aiColourComboBox->setCurrentIndex(0);
	else aiColourComboBox->setCurrentIndex(1);
	aiColourHorizontalLayout->addWidget(aiColourComboBox);

	okButton = new QPushButton("Ok", this);
	verticalLayout->addWidget(okButton);

	connect(okButton, SIGNAL(clicked()), this, SLOT(doneButtonCallback()));
}

Options OptionsMenuDialog::getOptions()
{
	Options newOptions;
	newOptions.isAi = isAiCheckbox->isChecked();
	if (aiColourComboBox->currentText() == "White") newOptions.aiColour = white;
	else newOptions.aiColour = black;
	
	return newOptions;
}

void OptionsMenuDialog::doneButtonCallback()
{
	emit accept();
}

Options::Options()
{
	isAi = false;
	aiColour = white;
}
