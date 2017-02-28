#pragma once

#include "ui_csvEditor.h"


#include <QDialog>
#include "qabstractbutton.h"
#include "vector.h"

#include "PropModel.h"
#include "qtableview.h"
#include "tablewidgetDateItem.h"

class MainWindow;

namespace Ui {
	class csvEditor;
}

class csvEditor : public QDialog
{
	Q_OBJECT

public:
	explicit csvEditor(MainWindow *parent, bool precipitationSeries, QString title = "Editor", QString fileName = "Untitled.csv", QTableView* tableProp = 0, QModelIndex index = QModelIndex(), QString fileType = "Comma Seperated File (*.csv)", bool modal = false);
	~csvEditor() {};
	void load(QString fileName);
	void save(QString fileName);
	void append(QString text);
	void append(QStringList text);
	void append(vector<string> text);
	void setHtml(vector<string> text);
	void operator()(QString text) { append(text); };
	void writetotempfile();
	QString text();
	private slots:
	void on_buttonBox_clicked(QAbstractButton * button);
	void on_pushButton_clicked();

public slots:
void showContextMenu(const QPoint&);
void on_horizontal_sectionClicked(int);
void on_horizontal_sectionDoubleClicked(int);


private:
	Ui::csvEditor *ui;
	//QString logfilename = "";
	MainWindow *mainWindow;
	QString wtitle, fileName, fileType;
	bool dataChanged;
	bool precipitation;
	QTableView* table;
	QModelIndex index;
};

