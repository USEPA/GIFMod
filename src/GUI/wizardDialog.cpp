#ifdef GIFMOD
#include "wizardDialog.h"
#include "ui_wizardDialog.h"
#include "GWidget.h"

wizardDialog::wizardDialog(QWidget *parent, GraphWidget* gWidget, QList<command>& commands):
    QDialog(parent),
    ui(new Ui::wizardDialog)
{
	ui->setupUi(this);
	for (int i = 0; i < commands.count(); i++)
	{
		if (commands[i].askUser)
		{
			for (int j = 0; j < commands[i].properties.count(); j++)
			{

			}
		}
	}

	this->show();
}

wizardDialog::~wizardDialog()
{
	delete this;
}

#endif