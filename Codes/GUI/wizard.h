#pragma once

#include <QDialog>
#include "qabstractbutton.h"
#include "vector.h"
#include "qrasterwindow.h"
#include "wizardItem.h"
struct templateIcon
{
	QString name, csvFile, description;
	QPixmap icon;
	//	QIcon icon;
};

class MainWindow;

namespace Ui {
class wizard;
}

class wizard : public QDialog
{
  Q_OBJECT
  
public:
  wizard(MainWindow *parent);
  ~wizard() {};
  void loadCsvFile();
  void assignActions();
  void append(QString text);
  void append(QStringList text);
  void append(vector<string> text);
  void setHtml(vector<string> text);
  void operator()(QString text) { append(text); };
  void writetotempfile();
  QString text();
private slots:
//void on_buttonBox_clicked(QAbstractButton * button);

private:
  Ui::wizard *ui;
  QString logfilename = "";
  MainWindow *mainWindow;
  QString wtitle, fileType;
  QList<templateIcon> templatesList;
  QGraphicsScene *scene;
};

