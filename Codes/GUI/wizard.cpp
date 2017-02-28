#include "wizard.h"
#include "ui_wizard.h"
#include "qdatetime.h"
#include "qfiledialog.h"
#include "mainwindow.h"
#include "GWidget.h"
#include "qmessagebox.h"
#include <iostream>
#include <fstream>
#include "qdir.h"
#include "qgraphicsWidget.h"
#include "wizardItem.h"


wizard::wizard(MainWindow *parent) :
  QDialog(parent),
  ui(new Ui::wizard)
{
  ui->setupUi(this);
  //wtitle = title;
  setWindowTitle("Select a template to buid a model from");
  this->fileType = "GIFt";
  mainWindow = parent;
  loadCsvFile();
  scene = new QGraphicsScene(this);
  scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  scene->setSceneRect(0, 0, 2000, 2000);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setCacheMode(ui->graphicsView->CacheBackground);
  ui->graphicsView->setViewportUpdateMode(ui->graphicsView->BoundingRectViewportUpdate);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setTransformationAnchor(ui->graphicsView->AnchorUnderMouse);
//  QObject::connect(MainGraphicsScene, SIGNAL(changed(const QList<QRectF>)), this, SLOT(sceneChanged()));
  




  //scene = ui->graphicsView->scene();

  assignActions();
  show();
 // ui->graphicsView->setS(scene);
  return;

  //setModal(true);
  //ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
  
}
void wizard::loadCsvFile()

{
ifstream file(QDir::currentPath().toStdString() + "/GUITemplates.csv");

if (!file.good())
return;
string line;
QString folder = QDir::currentPath() + "/icons/wizrds/";
while (!file.eof())
{
	getline(file, line);
	templateIcon tI;
	QStringList text = QString::fromStdString(line).split(",");
	if (text.size() == 4)
	{
		tI.name = text[0].trimmed();
		QString csvFilename = folder + text[1].trimmed();
		tI.csvFile = csvFilename;
		QString iconFilename = folder + text[2].trimmed();
//		tI.icon = QIcon(iconFilename);
		tI.icon = QPixmap(iconFilename);
		tI.description = text[3].trimmed();
		templatesList.append(tI);
	}
}

file.close();
}
void wizard::assignActions()
{
	for (int i = 0; i < templatesList.count(); i++)
	{
		QAction *a = new QAction(templatesList[i].icon, templatesList[i].name, this);// ui->scrollArea);
		scene->addPixmap(templatesList[i].icon);// ui->scrollArea->addAction(a);
		wizardItem *item;
//		item = new wizardItem(this, templatesList[i].name, "", templatesList[i].description, templatesList[i].icon);
		scene->addItem(item);
	}
	ui->graphicsView->repaint();
}


void wizard::append(QString text)
{
	//ui->textEdit->append(QTime::currentTime().toString().append(", ").append(text));
	//QApplication::processEvents();
}
void wizard::append(vector<string> text)
{
	QStringList list;
	for (int i = 0; i < text.size(); i++)
		list.append(QString::fromStdString(text[i]));
	append(list);
}
void wizard::setHtml(vector<string> text)
{
	QString html;
	for (int i = 0; i < text.size(); i++)
		html.append(QString::fromStdString(text[i]));
	//ui->textEdit->setHtml(html);
}
void wizard::append(QStringList text)
{
	//for each (QString line in text)
	//	ui->textEdit->append(line);
}

/*void wizard::on_buttonBox_clicked(QAbstractButton * button)
{
//	if (button->text() == "Reset")
//		ui->textEdit->clear();
	if (button->text() == "Save")
	{
/*		QString fileName = (!logfilename.isEmpty()) ? logfilename : QFileDialog::getSaveFileName(this,
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
//			XString text = ui->textEdit->toPlainText();
//			out << text.reformBack().toStdString();
			out.close();
			logfilename = fileName;
//			setWindowTitle(OnlyFilename(fileName).append(" - ").append(wtitle));
			return;
		//}
	}
}*/
void wizard::writetotempfile()
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
//	XString text = ui->textEdit->toPlainText();
//	out << text.reformBack().toStdString();
	out.close();
	return;
}
QString wizard::text()
{
	return "";// ui->textEdit->toPlainText();
}
