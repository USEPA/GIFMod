#ifdef GIFMOD
#include "ObservedConstituentWindow.h"
#include "ui_ObservedConstituentWindow.h"
#include "qtableview.h"
#include "node.h"
#include "GWidget.h"
#include "ObservedConstituentTableModel.h"
#include "ConstituentDelegate.h"

//ObservedConstituentWindow::ObservedConstituentWindow(QWidget *parent) :
//QDialog(parent),
//ui(new Ui::ObservedConstituentWindow)
//{}
ObservedConstituentWindow::ObservedConstituentWindow(QWidget *parent, GraphWidget *gWidget) :
    QDialog(parent),
    ui(new Ui::ObservedConstituentWindow)
{
    ui->setupUi(this);
	QStringList Constituents = gWidget->EntityNames("Constituent");
//	this->node = node;
	QTableView *table = new QTableView;
	model = new ObservedConstituentTableModel(table,gWidget);
	ConstituentDelegate * pDelegate = new ConstituentDelegate(this, gWidget);
/*	ui->tableView->setModel(model);
	ui->tableView->setItemDelegate(pDelegate);
	ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AllEditTriggers
		| QAbstractItemView::SelectedClicked);
	ui->tableView->setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);// | Qt::WindowStaysOnBottomHint);
	ui->tableView->show();
*/

	table->setModel(model);
	table->setItemDelegate(pDelegate);
	table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AllEditTriggers
	| QAbstractItemView::SelectedClicked);
	table->setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);// | Qt::WindowStaysOnBottomHint);
	ui->horizontalLayout_3->addWidget(table);
//	connect(ui->pushButton, SIGNAL(clicked()), model, SLOT(removeCurrentItem()));

	this->show();
}
QString ObservedConstituentWindow::cg()
{
	QString cg, Constituent, Particle, Model;
	Constituent = model->index(0, 0).data().toString();
	Particle = model->index(0, 1).data().toString();
	Model = model->index(0, 2).data().toString();
	
	cg.append("cg[");
	cg.append(Constituent);
	if (Particle != "") {
		cg.append(":");
		cg.append(Particle);
	}
	if (Model != "") {
		cg.append(":");
		cg.append(Model);
	}
	cg.append("]");
	return cg;
}
ObservedConstituentWindow::~ObservedConstituentWindow()
{
	delete this;
}

#endif