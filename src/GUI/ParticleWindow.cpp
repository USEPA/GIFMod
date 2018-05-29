#include "ParticleWindow.h"
#include "ui_ParticleWindow.h"
#include "qtableview.h"
#include "node.h"
#include "GWidget.h"
#include "ParticleTableModel.h"
#include "ParticleDelegate.h"
//#include "mainwindow.h"

//ParticleWindow::ParticleWindow(QWidget *parent) :
//QDialog(parent),
//ui(new Ui::ParticleWindow)
//{}
ParticleWindow::ParticleWindow(QWidget *parent, GraphWidget *gWidget, Node *node, QString &experiment) :
    QDialog(parent),
    ui(new Ui::ParticleWindow)
{
	gWidget->log(QString("Particle initial condition window started for block %1, experiment %2.").arg(node->Name()).arg(experiment));
	ui->setupUi(this);
	QStringList particles = gWidget->EntityNames("Particle");
	this->node = node;
	QTableView *table = new QTableView;
	model = new ParticleTableModel(node, table,gWidget);
	ParticleDelegate * pDelegate = new ParticleDelegate(this, gWidget);
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
void ParticleWindow::on_pushButton_clicked()
{
	model->removeCurrentItem();
}

ParticleWindow::~ParticleWindow()
{
	delete model;
	delete this;
}

