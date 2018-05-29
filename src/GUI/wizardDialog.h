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
#include "ui_wizardDialog.h"
#include "GWidget.h"


namespace Ui
{
class wizardDialog;
}

class wizardDialog : public QDialog
{
    Q_OBJECT

public:
	explicit wizardDialog(QWidget *parent, GraphWidget* gWidget, QList<command>& commands);
	~wizardDialog();

public slots:

private slots :

private:
    Ui::wizardDialog *ui;
};
#endif