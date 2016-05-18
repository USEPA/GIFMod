#ifdef WQV
#pragma once

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
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <unittextbox.h>

#include "ui_gridWindow.h"
#include "xstring.h"
#include <QDoubleValidator>

namespace Ui
{
class gridWindow;
}

class gridWindow : public QDialog
{
    Q_OBJECT

public:
	explicit gridWindow(QWidget *parent, XString z0, QString type = "2D Vertical", bool canChangeType = true, XString length = 0.0, XString width = 0.0, XString height = 0.0);
	~gridWindow();
	QMap<QString, XString> getGridInfo();
	Ui::gridWindow *ui;

public slots:

private slots :
	void accept();
	void reject();
	void on_columnsSpin_valueChanged(int columns);
	void on_rowsSpin_valueChanged(int rows);
	void on_buttonBox_clicked(QAbstractButton * button);
	void on_radioButtonH_clicked(bool checked);
	void on_radioButtonV_clicked(bool checked);
	void on_radioButtonH_toggled(bool checked);
	void on_radioButtonV_toggled(bool checked);

	void closeEvent(QCloseEvent *event);

signals:

private:
	XString z0, height;
	bool accepted = false;
	QDoubleValidator *validator;
	QMap<QString, XString> results;
};
#endif