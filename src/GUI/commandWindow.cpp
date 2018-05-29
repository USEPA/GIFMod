#include "commandWindow.h"
#include "ui_commandWindow.h"
#include "qdatetime.h"
#include "qfiledialog.h"
#include "mainwindow.h"
#include "GWidget.h"
#include "qmessagebox.h"
#include <iostream>
#include <fstream>
#include "texToHtml.h"

#include "scriptingengine.h"
commandWindow::commandWindow(GraphWidget *parent) :
	QDialog(parent),
	ui(new Ui::commandWindow)
{
	
	ui->setupUi(this);
	ui->textEdit->setEnabled(false);
	SetCompleter(ui->lineEdit->toPlainText());
	
	//completer = new QCompleter(this);
	//completer->setModel(modelFromFile(":/resources/wordlist.txt"));

	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	ui->lineEdit->setCompleter(completer);
	commands = new QList<CCommand>;

	connect(ui->lineEdit, SIGNAL(returnPressed(QKeyEvent *)), this, SLOT(newCommandSubmitted()));
	connect(ui->lineEdit, SIGNAL(navigate(QKeyEvent *)), this, SLOT(commandKeyRelease(QKeyEvent *)));
	connect(ui->lineEdit, SIGNAL(DotPressed(QKeyEvent *)), this, SLOT(commandDotRelease(QKeyEvent *)));
	connect(ui->lineEdit, SIGNAL(SpacePressed(QKeyEvent *)), this, SLOT(commandSpaceRelease(QKeyEvent *)));
	connect(ui->lineEdit, SIGNAL(CommaPressed(QKeyEvent *)), this, SLOT(commandCommaRelease(QKeyEvent *)));
	
	// wtitle = title;
	// setWindowTitle(wtitle);
	// this->helpFile = helpFile;
	 //alternateHelpFile = parent->defaultDir().toStdString() + "/" + helpFile;
	this->parent = parent;
	//  ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
   // ui->textEdit->
   // if (!loadHelpFile(alternateHelpFile))
  //	  loadHelpFile(helpFile);
   // ui->save->hide();
   // updateButtons();
}
void commandWindow::newCommandSubmitted()
{
	commands->append(ui->lineEdit->text());
	QString result;
	commandsHistory.append(ui->lineEdit->text());
	nextIndex = commandsHistory.count() - 1;
	prevIndex = nextIndex;
	//if (ui->toolButtonBasicMode->isChecked()) {
		result = parent->runCommand(commands->value(commands->size()-1)).toString();
	//}
	/*else {
		QString code = ui->lineEdit->text().toStdString().data();		
		ScriptingEngine::instance()->eval(code, result);
		// result = parent->mainWindow->m_scriptPad.data()->runScript(ui->lineEdit->text(),false);
	}*/
	ui->textEdit->append(QString(">%1").arg(ui->lineEdit->text()));
	ui->textEdit->append(result);
	ui->textEdit->append("\n");

	//qDebug() << ui->textEdit->toPlainText();
	ui->lineEdit->setText("");
}

//void commandWindow::writetotempfile()
//{
//}

void commandWindow::SetCompleter(QString s)
{
	QStringList wordList;
	if (s.trimmed() == "")
	{
		wordList << "add" << "zoom"<< "help" << "clear" << "list"<<"add";
	}
	if (s.trimmed() == "add")
	{
		wordList << "pond" << "soil" << "storage" << "catchment" << "darcy" << "stream" << "plant";
	}
	if (s.trimmed().right(1)==",")
		if (s.trimmed().left(3) == "add")
		{
			wordList = parent->mList->extract_props_for_type(s.trimmed().mid(3, s.length() - 4));
		}
	

	completer = new QCompleter(wordList, this);

}

QStringList commandWindow::extract_props_for_type(QString type)
{
	return parent->mList->extract_props_for_type(type);
}

QStringList commandWindow::extract_units_for_prop(QString type, QString property)
{
	return parent->mList->extract_units_for_prop(type, property);
}

QStringList commandWindow::extract_nodes(QString type)
{
	if (type == "")
		return parent->nodeNames();
	else
	{
		QStringList outlist;
		for (int i = 0; i < parent->Nodes().size(); i++)
			if (parent->Nodes()[i]->ObjectType().ObjectType == type)
				outlist.append(parent->Nodes()[i]->Name());
		return outlist;
	}
}

void commandWindow::append(QString s)
{
	ui->textEdit->append(s);
}


void commandWindow::commandDotRelease(QKeyEvent *e)
{
	//qDebug() << "dot";
}

void commandWindow::commandCommaRelease(QKeyEvent *e)
{
	SetCompleter(ui->lineEdit->toPlainText());
	ui->lineEdit->setCompleter(completer);
}

void commandWindow::commandSpaceRelease(QKeyEvent * e)
{
	SetCompleter(ui->lineEdit->toPlainText());
	ui->lineEdit->setCompleter(completer);
}

void commandWindow::commandKeyRelease(QKeyEvent *e)
{
	//qDebug() << prevIndex << nextIndex;

	/*	if (prevIndex < 0)
			prevIndex = 0;
		if (nextIndex > commandsHistory.count() - 1)
			nextIndex = commandsHistory.count() - 1;
		//qDebug() << prevIndex << nextIndex;
		*/
	int index;
	if (e->key() == Qt::Key_Up)
	{
		index = prevIndex--;
		prevIndex = max(0, prevIndex);
		nextIndex = index + 1;
		nextIndex = min(commandsHistory.count() - 1, nextIndex);
	}
	if (e->key() == Qt::Key_Down)
	{
		index = nextIndex++;
		nextIndex = min(commandsHistory.count() - 1, nextIndex);
		prevIndex = index - 1;
		prevIndex = max(0, prevIndex);
	}

	//qDebug() << prevIndex << index << nextIndex;
	ui->lineEdit->setText(commandsHistory[index]);
}

void commandWindow::on_toolButtonScriptMode_toggled(bool checked)
{
}

void commandWindow::on_toolButtonBasicMode_toggled(bool checked)
{
}
