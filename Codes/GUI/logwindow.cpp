#include "logWindow.h"
#include "ui_logwindow.h"
#include "qdatetime.h"
#include "qfiledialog.h"
#include "mainwindow.h"
#include "GWidget.h"
#include "qmessagebox.h"
#include <iostream>
#include <fstream>

logWindow::logWindow(MainWindow *parent, QString title, QString fileType, bool modal) :
  QDialog(parent),
  ui(new Ui::logWindow)
{
  ui->setupUi(this);
  wtitle = title;
  setWindowTitle(wtitle);
  this->fileType = fileType;
  mainWindow = parent;
  setModal(modal);
  ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);

}
void logWindow::append(QString text)
{
	ui->textEdit->append(QTime::currentTime().toString().append(", ").append(text));
	QApplication::processEvents();
}
void logWindow::append(vector<string> text)
{
	QStringList list;
	for (int i = 0; i < text.size(); i++)
		list.append(QString::fromStdString(text[i]));
	append(list);
}
void logWindow::setHtml(vector<string> text)
{
	QString html;
	for (int i = 0; i < text.size(); i++)
		html.append(QString::fromStdString(text[i]));
	ui->textEdit->setHtml(html);
}
void logWindow::append(QStringList text)
{
	for each (QString line in text)
		ui->textEdit->append(line);
}

void logWindow::on_buttonBox_clicked(QAbstractButton * button)
{
	if (button->text() == "Reset")
		ui->textEdit->clear();
	if (button->text() == "Save")
	{
		QString fileName = (!logfilename.isEmpty()) ? logfilename : QFileDialog::getSaveFileName(this,
			tr("Save ").append(mainWindow->applicationName), mainWindow->modelPathname(),
			fileType+";;All Files (*)");
		if (fileName.isEmpty())
			return;
		else {
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly)) {
				QMessageBox::information(this, tr("Unable to open file"),
					file.errorString());
				return;
			}
			
//			QDataStream out(&file);
			std::ofstream out;
			out.open(fileName.toStdString());

			out << mainWindow->applicationName.toLatin1().toStdString() << "\n";
			XString text = ui->textEdit->toPlainText();
			out << text.reformBack().toStdString();
			out.close();
			logfilename = fileName;
			setWindowTitle(OnlyFilename(fileName).append(" - ").append(wtitle));
			return;
		}
	}
}
void logWindow::writetotempfile()
{
	QString fileName = (!logfilename.isEmpty()) ? logfilename : mainWindow->modelPathname() + "/temp.log";
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
	return;
}
QString logWindow::text()
{
	return ui->textEdit->toPlainText();
}
