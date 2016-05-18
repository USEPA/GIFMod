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
#include "ui_ObservedConstituentWindow.h"

class ObservedConstituentTableModel;
class GraphWidget;

namespace Ui
{
class ObservedConstituentWindow;
}

class ObservedConstituentWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ObservedConstituentWindow(QWidget *parent, GraphWidget* gWidget);
//	explicit ObservedConstituentWindow(QWidget *parent = 0);
	~ObservedConstituentWindow();
	QString cg();
public slots:

private slots :
//	void on_action_OK_clicked();

private:
    Ui::ObservedConstituentWindow *ui;
	Node *node;
	ObservedConstituentTableModel *model;


};
