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
#include "ui_ExchangeParametersWindow.h"

class ExchangeParametersTableModel;
class GraphWidget;
class Entity;
namespace Ui
{
class ExchangeParametersWindow;
}

class ExchangeParametersWindow : public QDialog
{
    Q_OBJECT

public:
	explicit ExchangeParametersWindow(QWidget *parent, GraphWidget* gWidget, Entity *entity);// , QString &experiment);
//	explicit ExchangeParametersWindow(QWidget *parent = 0);
	~ExchangeParametersWindow();

public slots:

private slots :
	void on_pushButton_clicked();

private:
    Ui::ExchangeParametersWindow *ui;
	Entity *entity;
	ExchangeParametersTableModel *model;


};
