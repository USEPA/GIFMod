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
#include "ui_ParticleWindow.h"

class ParticleTableModel;
class GraphWidget;

namespace Ui
{
class ParticleWindow;
}

class ParticleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ParticleWindow(QWidget *parent, GraphWidget* gWidget, Node *node, QString &experiment);
//	explicit ParticleWindow(QWidget *parent = 0);
	~ParticleWindow();

public slots:

private slots :
	void on_pushButton_clicked();

private:
    Ui::ParticleWindow *ui;
	Node *node;
	ParticleTableModel *model;


};
