#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include "vector.h"
#include "navigationList.h"
#include "GWidget.h"

//class MainWindow;

namespace Ui {
class helpWindow;
}

class helpWindow : public QDialog
{
  Q_OBJECT
  
public:
  helpWindow(GraphWidget *parent, QString title = "Help", string helpFile= "help.txt");
  helpWindow() {};
  ~helpWindow() {};
  void append(QString text);
  void append(QStringList text);
  void append(vector<string> text);
  //void appendLine(string text);

  void setHtml(vector<string> text);
  void setHtml(QString html);
  void operator()(QString text) { append(text); };
  void showHelp(int code, string variableName, bool appendtoList = true);
  vector<string> search(int code);
  QString search2Html (int code);
  void writetotempfile();
  void updateButtons();
  bool loadHelpFile(string helpFile);
private slots:

	void on_back_clicked();
	void on_forward_clicked();
	void on_save_clicked();

private:
  Ui::helpWindow *ui;
 // MainWindow *mainWindow;
  GraphWidget* parent;
  QString wtitle;
  string helpFile, alternateHelpFile;
  navigationList codes;
  QStringList text;
  int showingItemCode;
  QString showingVariableName;
};

#endif // HELPWINDOW_H
