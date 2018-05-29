#ifndef commandWindow_H
#define commandWindow_H

#include <QDialog>
#include <vector>
#include "navigationList.h"
#include "GWidget.h"
//#include "CommandTextEdit.h"
#include "Command.h"

using namespace std;

namespace Ui {
class commandWindow;
}

class commandWindow : public QDialog
{
  Q_OBJECT
  
public:
  commandWindow(GraphWidget *parent);
  commandWindow() {};
  ~commandWindow() {};
  QCompleter *completer;
  void SetCompleter(QString s);
  QStringList extract_props_for_type(QString type);
  QStringList extract_units_for_prop(QString type, QString property);
  QStringList extract_nodes(QString type = "");
  QList<CCommand> *commands;
  void append(QString s);
//  void append(QString text);
//  void append(QStringList text);
//  void append(vector<string> text);

//  void setHtml(vector<string> text);
//  void setHtml(QString html);
//  void operator()(QString text) { append(text); };
//  void showHelp(int code, string variableName, bool appendtoList = true);
//  vector<string> search(int code);
//  QString search2Html (int code);
//  void writetotempfile();
//  void updateButtons();
//  bool loadHelpFile(string helpFile);
private slots:

//	void on_back_clicked();
//	void on_forward_clicked();
//	void on_save_clicked();
	void newCommandSubmitted();
	void commandKeyRelease(QKeyEvent *);
	void commandDotRelease(QKeyEvent * e);
	void commandCommaRelease(QKeyEvent * e);
	void commandSpaceRelease(QKeyEvent * e);
    void on_toolButtonScriptMode_toggled(bool checked);

    void on_toolButtonBasicMode_toggled(bool checked);

private:
  Ui::commandWindow *ui;
 // MainWindow *mainWindow;
  GraphWidget* parent;
  QString wtitle;
  string helpFile, alternateHelpFile;
  QStringList commandsHistory;
  int prevIndex = -1, nextIndex = -1;
  QStringList text;
  int showingItemCode;
  QString showingVariableName;
};

#endif // commandWindow_H
