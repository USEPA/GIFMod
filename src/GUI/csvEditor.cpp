#include "csvEditor.h"
#include "ui_csvEditor.h"

#include "qdatetime.h"
#include "qfiledialog.h"
#include "mainwindow.h"
#include "GWidget.h"
#include "qmessagebox.h"
#include <iostream>
#include <fstream>
#include "qmenu.h"
//#include "qheaderview.h"
#include <QInputDialog>

csvEditor::csvEditor(MainWindow *parent, bool precipitationSeries, QString title, QString fileName, QTableView* tableProp, QModelIndex index, QString fileType, bool modal):
	QDialog(parent),
	ui(new Ui::csvEditor)
{
	ui->setupUi(this);
	wtitle = title;
	setWindowTitle(wtitle);
	this->fileType = fileType;
	this->fileName = fileName;
	mainWindow = parent;
	setModal(modal);
	table = tableProp;
	this->index = index;

	precipitation = precipitationSeries;

	int minCol = (precipitation) ? 3 : 2;
	if (ui->tableWidget->columnCount() < minCol)
		ui->tableWidget->setColumnCount(minCol);
	if (ui->tableWidget->rowCount() < 1)
		ui->tableWidget->setRowCount(1);

	if (precipitation)
		ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Start time" << "End time" << "Volume");
	else 
		ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Time" << "Value");
	for (int i = 0; i < ui->tableWidget->rowCount(); i++)
		for (int j = 0; j < ui->tableWidget->columnCount(); j += 2)
			ui->tableWidget->setItem(i, j, new TableWidgetDateItem());

	load(fileName);
	show();
//	load("D:/a.csv");
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
	
	if (!precipitation)
		connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(on_horizontal_sectionDoubleClicked(int)));
}

void csvEditor::showContextMenu(const QPoint&p)
{
//	QTableWidgetItem *item = ui->tableWidget->itemAt(p);
//	if (!item)
//		return;
//	int row = ui->tableWidget->row(item);
//	int col = ui->tableWidget->column(item);

	int row = ui->tableWidget->rowAt(p.y());
	int col = ui->tableWidget->columnAt(p.x());

	if (row == -1 || col == -1)
		return;
	QMenu menu;
	menu.addAction("insert row");
	menu.addAction("delete row");
	if (!precipitation)
	{
		menu.addAction("insert column");
		menu.addAction("delete column");
	}

	QAction *ac = menu.exec(mapToGlobal(p));
	if (!ac)
		return;

	if (ac->text() == "insert row")
	{
		ui->tableWidget->insertRow(row);
		for (int i = 0; i < ui->tableWidget->columnCount(); i+=2)
			ui->tableWidget->setItem(row, i, new TableWidgetDateItem());
	}

	if (ac->text() == "insert column")
	{
		if ((col % 2))
			col++;
		ui->tableWidget->insertColumn(col);
		ui->tableWidget->setHorizontalHeaderItem(col, new QTableWidgetItem("Value"));
		ui->tableWidget->insertColumn(col);
		ui->tableWidget->setHorizontalHeaderItem(col, new QTableWidgetItem("Time"));
		for (int i = 0; i < ui->tableWidget->rowCount(); i++)
			ui->tableWidget->setItem(i, col, new TableWidgetDateItem());
	}
	if (ac->text() == "delete row")
		ui->tableWidget->removeRow(row);
	
	if (ac->text() == "delete column")
	{
		if (ui->tableWidget->columnCount() == 2)
			return;
		if (col % 2)
			col--;
		ui->tableWidget->removeColumn(col + 1);
		ui->tableWidget->removeColumn(col);
	}
}

void csvEditor::load(QString fileName)
{
	if (fileName.size())
		this->fileName = fileName;
	ifstream file(fileName.toStdString());
	if (!file.good())
		return;
	string line;
	ui->tableWidget->clear();
	int row = 0;
	while (file.eof() == false)
	{
		getline(file, line);
		QStringList list = QString::fromStdString(line).split(",");
		int addRow;
		for (int col = 0; col < list.size(); col++)
		{
			addRow = 1;
			if (ui->tableWidget->rowCount() < row + 1)
				ui->tableWidget->setRowCount(row + 1);
			if (ui->tableWidget->columnCount() < col + 1)
				ui->tableWidget->setColumnCount(col + 1);
			if (list[0].toLower().contains("names") && !precipitation)
			{
				if (col > 0) {
					if (ui->tableWidget->columnCount() < 2 * (list.size() - 1))
						ui->tableWidget->setColumnCount(2 * (list.size() - 1));

					ui->tableWidget->setHorizontalHeaderItem((col -1) * 2, new QTableWidgetItem("time"));
					ui->tableWidget->setHorizontalHeaderItem((col -1)* 2 + 1, new QTableWidgetItem(list[col]));
				}
				addRow = 0;;
			}
			else
			{ 
				if (!precipitation)
				{
					if (col % 2 == 0) // time columns
						ui->tableWidget->setItem(row, col, new TableWidgetDateItem(list[col])); 
					else //value column
						ui->tableWidget->setItem(row, col, new QTableWidgetItem(list[col]));
				}
				else //precipitation table
				{
					if (col < 2 ) // time columns
						ui->tableWidget->setItem(row, col, new TableWidgetDateItem(list[col]));
					else //value column
						ui->tableWidget->setItem(row, col, new QTableWidgetItem(list[col]));
				}

			}
			//qDebug() << row << col;
		}
		row += addRow;
	}
	file.close();
	dataChanged = false;
}

void csvEditor::save(QString fileName)
{
	ofstream file(fileName.toStdString());
	if (!file.good())
		return;

	int checkHeaders = 0;
	for (int j = 0; j < ui->tableWidget->columnCount(); j++)
	{
		checkHeaders += (ui->tableWidget->horizontalHeaderItem(j)) ? 1 : 0;
	}
	for (int j = 0; j < ui->tableWidget->columnCount(); j++)
		if (ui->tableWidget->horizontalHeaderItem(j)->text() == "Time")
		{
			for (int i = 0; i < ui->tableWidget->rowCount(); i++)
			{
				if (ui->tableWidget->item(i, j)->data(1000).toInt() == -1)
				{
					QMessageBox::critical(this, "Error", "There is undefined value(s) in 'Time' columns.\nTable could not be saved with undefined value(s).");
					return;
				}
			}
		}

		
	if (checkHeaders && !precipitation)
	{
		QStringList list;
		list << "names"; 
		for (int j = 1; j < ui->tableWidget->columnCount(); j +=2)
		{
			list.append(ui->tableWidget->horizontalHeaderItem(j) ? ui->tableWidget->horizontalHeaderItem(j)->text() : QString("value %1").arg((j + 1) / 2));
		}
		QString line = list.join(",");
		file << line.toStdString() << "\n";
	}

	for (int i = 0; i < ui->tableWidget->rowCount(); i++)
	{
		QStringList list;
		for (int j = 0; j < ui->tableWidget->columnCount(); j++)
		{
			//qDebug() << i << j;
			if (!precipitation) // time vaule tables
			{
				if (j%2==0)//time columns
					list.append(ui->tableWidget->item(i, j) ? ui->tableWidget->item(i, j)->data(1000).toString() : "");
				else// value columns
					list.append(ui->tableWidget->item(i, j) ? ui->tableWidget->item(i, j)->text() : "");
			}
			else // precipitation tables
			{
				if (j < 2)//time columns
					list.append(ui->tableWidget->item(i, j) ? ui->tableWidget->item(i, j)->data(1000).toString() : "");
				else// value columns
					list.append(ui->tableWidget->item(i, j) ? ui->tableWidget->item(i, j)->text() : "");
			}
			//qDebug() << i << j << list.last();
		}
		QString line = list.join(",");
		bool lastRow = (i == ui->tableWidget->rowCount() - 1) ? true : false;
		if (!lastRow)
			line.append("\n");
		file << line.toStdString() ;
	}
	file.close();
	dataChanged = false;
}


void csvEditor::append(QString text)
{
	//ui->textEdit->append(QTime::currentTime().toString().append(", ").append(text));
	//QApplication::processEvents();
}

void csvEditor::append(vector<string> text)
{
//	QStringList list;
//	for (int i = 0; i < text.size(); i++)
//		list.append(QString::fromStdString(text[i]));
//	append(list);
}

void csvEditor::setHtml(vector<string> text)
{
//	QString html;
//	for (int i = 0; i < text.size(); i++)
//		html.append(QString::fromStdString(text[i]));
///	ui->textEdit->setHtml(html);
}

void csvEditor::append(QStringList text)
{
//	for each (QString line in text)
//		ui->textEdit->append(line);
}

void csvEditor::on_pushButton_clicked()
{
		ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
		int row = ui->tableWidget->rowCount() - 1;
		for (int i = 0; i < ui->tableWidget->columnCount(); i += 2)
			ui->tableWidget->setItem(row, i, new TableWidgetDateItem());

}

	void csvEditor::on_buttonBox_clicked(QAbstractButton * button)
{
	if (button->text() == "Add row")
		ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
	if (button->text() == "Save")
	{
		QString file = (!(fileName.isEmpty() || fileName=="Unititled.csv")) ? fileName : QFileDialog::getSaveFileName(this,
			tr("Save ").append(mainWindow->applicationName), mainWindow->modelPathname().append("/Untitled.csv"),
			fileType + ";;All Files (*)");
		if (file.isEmpty())
			return;
		else {
			save(file);
			if (fileName.isEmpty() || fileName == "Unititled.csv")
				if (table)
					table->model()->setData(index, file, Qt::EditRole);
		}
	}

//			QFile file(fileName);
//			if (!file.open(QIODevice::WriteOnly)) {
//				QMessageBox::information(this, tr("Unable to open file"),
//					file.errorString());
//				return;
//			}

			//			QDataStream out(&file);
//			std::ofstream out;
//			out.open(fileName.toStdString());
//
//			out << mainWindow->applicationName.toLatin1().toStdString() << "\n";
//			XString text = ui->textEdit->toPlainText();
//			out << text.reformBack().toStdString();
//			out.close();
//			logfilename = fileName;
//			setWindowTitle(OnlyFilename(fileName).append(" - ").append(wtitle));
//			return;
//		}
//	}
}

void csvEditor::writetotempfile()
{
//	QString fileName = (!logfilename.isEmpty()) ? logfilename : mainWindow->modelPathname() + "/temp.log";
//	QFile file(fileName);
//	if (!file.open(QIODevice::WriteOnly)) {
///		return;
//	}
	//	QDataStream out(&file);
//	std::ofstream out;
//	out.open(fileName.toStdString());
//	out << mainWindow->applicationName.toStdString() + " temp log file." << "\n";
//	XString text = ui->textEdit->toPlainText();
//	out << text.reformBack().toStdString();
//	out.close();
//	return;
}

QString csvEditor::text()
{
	QString text;
	for (int i = 0; i < ui->tableWidget->rowCount(); i++)
	{
		QStringList list;
		for (int j = 0; j < ui->tableWidget->columnCount(); j++)
			list.append(ui->tableWidget->itemAt(i, j)->text());
		QString line = list.join(",");
		text.append(line).append("\n");
	}
	return text;
}

void csvEditor::on_horizontal_sectionClicked(int i)
{
	//qDebug() << i;
}

void csvEditor::on_horizontal_sectionDoubleClicked(int i)
{
	if (!(i % 2))
		return; //does not rename the time columns
	bool ok;
	QString currentHeader = (ui->tableWidget->horizontalHeaderItem(i)) ? ui->tableWidget->horizontalHeaderItem(i)->text() : "";
	QString text = QInputDialog::getText(this, QString("Column number %1 header").arg(i),
		tr("Header:"), QLineEdit::Normal,
		currentHeader, &ok);
	if (ok && !text.isEmpty())
		ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(text));
}
