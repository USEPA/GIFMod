#ifdef GIFMOD
#pragma once
//#include <>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "node.h"
#include "ui_ConstituentWindow.h"

class ConstituentTableModel;
class GraphWidget;

namespace Ui
{
class ConstituentWindow;
}

class ConstituentWindow : public QDialog
{
    Q_OBJECT

public:
	explicit ConstituentWindow(QWidget *parent, GraphWidget* gWidget, Node *node, QString &experiment);
//	explicit ConstituentWindow(QWidget *parent = 0);
	~ConstituentWindow();

public slots:

private slots :
	void on_pushButton_clicked();

private:
    Ui::ConstituentWindow *ui;
	Node *node;
	ConstituentTableModel *model;


};
#endif