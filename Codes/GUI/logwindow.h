#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include "qabstractbutton.h"
#include "vector.h"

class MainWindow;

namespace Ui {
class logWindow;
}

class logWindow : public QDialog
{
  Q_OBJECT
  
public:
  explicit logWindow(MainWindow *parent, QString title = "Log Window", QString fileType= "Logfile (*.log)", bool modal = false);
  ~logWindow() {};
  void append(QString text);
  void append(QStringList text);
  void append(vector<string> text);
  void setHtml(vector<string> text);
  void operator()(QString text) { append(text); };
  void writetotempfile();
  QString text();
private slots:
void on_buttonBox_clicked(QAbstractButton * button);

private:
  Ui::logWindow *ui;
  QString logfilename = "";
  MainWindow *mainWindow;
  QString wtitle, fileType;

};

#endif // LOGWINDOW_H
