#include "helpWindow.h"
#include "ui_helpwindow.h"
#include "qdatetime.h"
#include "qfiledialog.h"
#include "mainwindow.h"
#include "GWidget.h"
#include "qmessagebox.h"
#include <iostream>
#include <fstream>
#include "texToHtml.h"

helpWindow::helpWindow(GraphWidget *parent, QString title, string helpFile) :
  QDialog(parent),
  ui(new Ui::helpWindow)
{
  ui->setupUi(this);
  wtitle = title;
  setWindowTitle(wtitle);
  this->helpFile = helpFile;
  alternateHelpFile = parent->defaultDir().toStdString() + "/" + helpFile;
  this->parent = parent;
  //  ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
 // ui->textEdit->
  if (!loadHelpFile(alternateHelpFile))
	  loadHelpFile(helpFile);
  ui->save->hide();
  updateButtons();
}
bool helpWindow::loadHelpFile(string helpFile)
{
	ifstream file(helpFile);
	if (!file.good())
		return false;
	string line;
	text.clear();
	while (file.eof() == false)
	{
		getline(file, line);
		text.push_back(QString::fromStdString(line));
	}
	file.close();
	return true;
}

void helpWindow::updateButtons()
{
	ui->back->setEnabled(codes.backActive());
	ui->forward->setEnabled(codes.forewardActive());
}

void helpWindow::append(QString text)
{
	ui->textEdit->append(QTime::currentTime().toString().append(", ").append(text));
	QApplication::processEvents();
}
void helpWindow::append(vector<string> text)
{
	QStringList list;
	for (int i = 0; i < text.size(); i++)
		list.append(QString::fromStdString(text[i]));
	append(list);
}

void helpWindow::setHtml(QString html)
{
//	QString Html = QString::fromStdString(html);
	ui->textEdit->setHtml(html);
	qDebug() << ui->textEdit->toHtml();
}

void helpWindow::setHtml(vector<string> text)
{
	QString html;
	for (int i = 0; i < text.size(); i++)
		html.append(QString::fromStdString(text[i]));
	ui->textEdit->setHtml(html);
}
void helpWindow::append(QStringList text)
{
	for each (QString line in text)
		ui->textEdit->append(line);
}
void helpWindow::showHelp(int code, string variableName, bool appendtoList)
{
	if (appendtoList)
		codes.append(code, variableName);

	ui->textEdit->setText("");
	showingVariableName = QString::fromStdString(variableName);
	//ui->textEdit->append(showingVariableName);
	setWindowTitle(QString("Help: %1").arg(showingVariableName));
	setHtml(search2Html(code));
	updateButtons();
	show();
}
vector<string> helpWindow::search(int code)
{
#ifdef GWA
	showingItemCode = code;
	vector<string> r;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i].contains("helpCode: ") && text[i].contains(QString::number(code)))
			for (int j = i + 1; j < text.size(); j++)
			{
				if (text[j].contains("helpCode: "))
					return r;
				r.push_back(text[j].toStdString());
			}
	}
	if (!r.size())
	{
		text.push_back(QString ("helpCode: %1").arg(code));
		r.push_back( "No content defined yet.");
	}
	return r;
#endif
#ifdef GIFMOD
	showingItemCode = code;
	vector<string> r;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i].contains("%help:") && text[i].contains(QString::number(code)))
			for (int j = i + 1; j < text.size(); j++)
			{
				r.push_back(text[j].toStdString());
				if (text[j].contains("%help:end"))
					return r;
			}
	}
	if (!r.size())
	{
		//text.push_back(QString("helpCode: %1").arg(code));
		r.push_back("No contents found for this subject.");
	}
	
	return r;
#endif

}
QString helpWindow::search2Html(int code)
{
#ifdef GWA
	showingItemCode = code;
	vector<string> r;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i].contains("helpCode: ") && text[i].contains(QString::number(code)))
			for (int j = i + 1; j < text.size(); j++)
			{
				if (text[j].contains("helpCode: "))
					return r;
				r.push_back(text[j].toStdString());
			}
	}
	if (!r.size())
	{
		text.push_back(QString("helpCode: %1").arg(code));
		r.push_back("No content defined yet.");
	}
	return r;
#endif
#ifdef GIFMOD
	showingItemCode = code;
	vector<string> r;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i].contains("%help:") && text[i].contains(QString::number(code)))
			for (int j = i + 1; j < text.size(); j++)
			{
				r.push_back(text[j].toStdString());
				if (text[j].contains("%help:end"))
				{
					string tex;
					for (int i = 0; i < r.size(); i++)
						tex = tex + r[i] + '\n';
					QString html = tex2Html(tex);

					return html;
				}
			}
	}
	if (!r.size())
	{
		//text.push_back(QString("helpCode: %1").arg(code));
		r.push_back("No contents found for this subject.");
	}
	return "";
#endif

}
void helpWindow::on_back_clicked()
{
	codes.back();
	int index = codes.currentIndex();
	showHelp(codes(index).toInt(), codes.desc(index).toString(), false);
}
void helpWindow::on_forward_clicked()
{
	codes.forward();
	int index = codes.currentIndex();
	showHelp(codes(index).toInt(), codes.desc(index).toString(), false);
}
void helpWindow::on_save_clicked()
{
	ofstream file(alternateHelpFile);
//	QString qhelp = QString::fromStdString(helpFile);
//	QString qhelp2 = parent->modelPathname() + OnlyFilename(qhelp);
//	ofstream file2(qhelp2.toStdString());
	if (!file.good()) // || !file2.good())
		return;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i].contains("helpCode: ") && text[i].contains(QString::number(showingItemCode)))
		{
			file << text[i].toStdString();// << "\n";
//			file2 << text[i].toStdString();// << "\n";
			QString a = ui->textEdit->toPlainText();
			QString b = a.right(a.size() - showingVariableName.size());

			file << b.toStdString() << "\n";
//			file2 << b.toStdString() << "\n";

			for (int j = i + 1; j < text.size(); j++)
			{
				if (text[j].contains("helpCode: "))
				{
					i = j - 1;
					break;
				}
			}
		}
		else
		{
			file << text[i].toStdString() << "\n";
//			file2 << text[i].toStdString() << "\n";
		}

	}
	file.close();
//	file2.close();
	loadHelpFile(alternateHelpFile);
	//loadHelpFile("new" + helpFile);
}
void helpWindow::writetotempfile()
{
/*	QString fileName = (!logfilename.isEmpty()) ? logfilename : mainWindow->modelPathname() + "/temp.log";
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
//	QDataStream out(&file);
	std::ofstream out;
	out.open(fileName.toStdString());
	out << mainWindow->applicationName.toStdString() + " temp log file." << "\n";
	XString text = ui->textEdit->toPlainText();
	out << text.reformBack().toStdString();
	out.close();
	return;*/
}
