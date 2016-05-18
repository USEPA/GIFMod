#include "observedcombobox.h"
#include "qdebug.h"
#include "ObservedConstituentWindow.h"

ObservedComboBox::ObservedComboBox(QWidget *parent)
	: QComboBox(parent)
{
	connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(textChanged(QString)));
}

ObservedComboBox::~ObservedComboBox()
{

}
void ObservedComboBox::textChanged(const QString & text)
{
	qDebug() << text;
	if (text == "Particle...")
	{
		QString txt;
//		ObservedConstituentWindow w(this, parent->main;
		txt = "Hello";
		if (findText(txt) == -1)	
			addItem (txt); 
//addItem(ObservedConstituentWindow::cg());
		setCurrentText(txt);
	}

	if (text == "Constituent...")
	{
		QString txt;
		txt = "Hello";
		if (findText(txt) == -1)
			addItem(txt);
		setCurrentText(txt);
	}

}