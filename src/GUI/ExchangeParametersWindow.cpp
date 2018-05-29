#include "ExchangeParametersWindow.h"
#include "ui_ExchangeParametersWindow.h"
#include "qtableview.h"
#include "entity.h"
#include "GWidget.h"
#include "ExchangeParametersTableModel.h"
#include "ParticleDelegate.h"
//#include "mainwindow.h"

//ExchangeParametersWindow::ExchangeParametersWindow(QWidget *parent) :
//QDialog(parent),
//ui(new Ui::ExchangeParametersWindow)
//{}
//ExchangeParametersWindow::ExchangeParametersWindow(QWidget *parent, GraphWidget *gWidget, Entity *entity, QString &experiment) :
ExchangeParametersWindow::ExchangeParametersWindow(QWidget *parent, GraphWidget *gWidget, Entity *entity) :
QDialog(parent),
    ui(new Ui::ExchangeParametersWindow)
{
	gWidget->log(QString("Solid aqueous exchange parametrs window started for constituent %1.").arg(entity->Name()));
	ui->setupUi(this);
	QStringList particles = QStringList() << "Soil" << gWidget->EntityNames("Particle");
	this->entity = entity;
	QTableView *table = new QTableView;
	model = new ExchangeParametersTableModel(entity, table,gWidget);
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
void ExchangeParametersWindow::on_pushButton_clicked()
{
	model->removeCurrentItem();
}

ExchangeParametersWindow::~ExchangeParametersWindow()
{
	delete model;
	delete this;
}

