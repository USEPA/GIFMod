#ifdef GIFMOD
#include "ConstituentWindowPlant.h"
#include "ui_ConstituentWindowPlant.h"
#include "qtableview.h"
#include "node.h"
#include "GWidget.h"
#include "ConstituentTableModelPlant.h"
#include "ConstituentDelegate.h"

//ConstituentWindow::ConstituentWindow(QWidget *parent) :
//QDialog(parent),
//ui(new Ui::ConstituentWindow)
//{}
ConstituentWindowPlant::ConstituentWindowPlant(QWidget *parent, GraphWidget *gWidget, Node *node, QString &experiment) :
    QDialog(parent),
    ui(new Ui::ConstituentWindowPlant)
{
	gWidget->log(QString("Limiting nutrient half saturation constant window started for block %1, experiment %2.").arg(node->Name()).arg(experiment));
	ui->setupUi(this);
	QStringList Constituents = gWidget->EntityNames("Constituent");
	this->node = node;
	QTableView *table = new QTableView;
	model = new ConstituentTableModelPlant(node, table,gWidget);
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
	table = NULL;
}
void ConstituentWindowPlant::on_pushButton_clicked()
{
	model->removeCurrentItem();
}

ConstituentWindowPlant::~ConstituentWindowPlant()
{
	ui = NULL;
	node = NULL;
	model = NULL;
	delete this;
}

#endif